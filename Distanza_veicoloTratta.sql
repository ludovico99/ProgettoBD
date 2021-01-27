CREATE PROCEDURE `Distanza_Veicolo_Fermata` (in var_TrattaCorrente char(5), in var_Fermata char(5), in var_DataPartenza date, in var_OrarioPartenza time, out var_Distanza float)
BEGIN
	declare var_VeicoloAttuale char(4) default null;
    declare var_LatitudineCoordinata float;
    declare var_LongitudineCoordinata float;
    declare var_LatitudineCoordinataSuccessiva float;
    declare var_LongitudineCoordinataSuccessiva float;
	declare var_LatitudinePartenza float;
    declare var_LongitudinePartenza float;
    declare var_LatitudineDestinazione float;
    declare var_LongitudineDestinazione float;
    declare var_NumeroOrdine_PrimoWaypoint int;
    declare var_NumeroOrdine_UltimoWaypoint int;
    declare var_DistanzaParziale float default 0;
    declare var_PrimaIterazione int default true;
    declare done int default false;
    declare cur1 cursor for select `Waypoint_Latitudine`,`Waypoint_Longitudine` from `AggregazioneTratta_Waypoint` where `TrattaStradale_CodiceIdentificativo` = var_TrattaCorrente and `Ordine` >=var_NumeroOrdine_PrimoWaypoint and  `Ordine`<= var_NumeroOrdine_UltimoWaypoint order by `Ordine` asc;
	declare cur2 cursor for select `Waypoint_Latitudine`,`Waypoint_Longitudine` from `AggregazioneTratta_Waypoint` where `TrattaStradale_CodiceIdentificativo` = var_TrattaCorrente and `Ordine` >=var_NumeroOrdine_PrimoWaypoint and  `Ordine`<= var_NumeroOrdine_UltimoWaypoint order by `Ordine` asc;
    declare continue handler for not found set done = true;
    
    declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;
    
	set transaction isolation level repeatable read;
    start transaction;
    set var_Distanza = 0;
	select `VeicoloPubblico_Matricola`
    from `TrattaReale`
    where `TrattaStradale` = var_TrattaCorrente and `DataPartenza`=var_DataPartenza and `OrarioPartenza`= var_OrarioPartenza into var_VeicoloAttuale;
	
    if var_VeicoloAttuale is null then 
		signal sqlstate '45006' set message_text="Informazioni inserite non valide";
	end if;
    
    select `Latitudine`,`Longitudine` 
    from  `VeicoloPubblico`
    where `Matricola`=var_VeicoloAttuale into var_LatitudinePartenza, var_LongitudinePartenza;
    
    select `Waypoint_Latitudine`,`Waypoint_Longitudine`
    from `Fermata`
    where `CodiceFermata` = var_Fermata into var_LatitudineDestinazione, var_LongitudineDestinazione;
    
    select `Ordine`
    from `AggregazioneTratta_Waypoint`
    where `Waypoint_Latitudine` = var_LatitudineDestinazione and `Waypoint_Longitudine`= var_LongitudineDestinazione and `TrattaStradale_CodiceIdentificativo` = var_TrattaCorrente into var_NumeroOrdine_UltimoWaypoint;
   
	set var_LatitudinePartenza = radians(var_LatitudinePartenza);
	set var_LongitudinePartenza = radians(var_LongitudinePartenza);
   
    open cur1;
    open cur2;
	read_loop: loop
	    fetch cur1 into var_LatitudineCoordinataSuccessiva, var_LongitudineCoordinataSuccessiva;
		if var_PrimaIterazione = false then
			fetch cur2 into var_LatitudineCoordinata , var_LongitudineCoordinata;
			set var_LatitudineCoordinata = radians(var_LatitudineCoordinata);
            set var_LongitudineCoordinata = radians( var_LongitudineCoordinata);
        end if;
		if done then
			leave read_loop;
		end if;
        set var_LatitudineCoordinataSuccessiva = radians(var_LatitudineCoordinataSuccessiva);
        set var_LongitudineCoordinataSuccessiva = radians(var_LongitudineCoordinataSuccessiva);
		if var_PrimaIterazione then
			set var_PrimaIterazione=false;
			select (2 * 6371 * asin(sqrt(pow(sin((var_LatitudineCoordinataSuccessiva-var_LatitudinePartenza)/2),2) + cos(var_LatitudinePartenza) * cos(var_LatitudineCoordinataSuccessiva)*pow(sin((var_LongitudineCoordinataSuccessiva-var_LongitudinePartenza)/2),2)))) into var_DistanzaParziale;
		else 
			select (2 * 6371 * asin(sqrt(pow(sin((var_LatitudineCoordinataSuccessiva-var_LatitudineCoordinata)/2),2) + cos(var_LatitudineCoordinata) * cos(var_LatitudineCoordinataSuccessiva)*pow(sin((var_LongitudineCoordinataSuccessiva-var_LongitudineCoordinata)/2),2)))) into var_DistanzaParziale;
		end if;
		set var_Distanza= var_Distanza + var_DistanzaParziale;
	end loop;
    close cur1;
    close cur2;
    if var_Distanza = 0 then
		signal sqlstate '45005' set message_text="Fermata inserita non valida o già superata";
    end if;
	commit;
END