CREATE PROCEDURE ProssimaPartenzaPrevista (in var_matricola char(4))
BEGIN
	declare var_OrarioPartenza time;
    declare var_DataPartenza date;
    declare var_TimeConfronto datetime;
    declare var_mindiff int;
    declare var_PrimaIterazione boolean default true;
    declare var_ProssimaPartenza_Data date default null;
    declare var_ProssimaPartenza_Ora time default null;
    declare var_TrattaIterazione char(5);
	declare var_TrattaStradale char(5) default null;
    
    declare done int default false;
	declare cur cursor for select  `TrattaStradale`, `DataPartenza`,`OrarioPartenza` from  `TrattaReale` where `VeicoloPubblico_Matricola`= var_matricola and `DataPartenza` >= curdate();
    declare continue handler for not found set done = true;

	declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;
    
    drop temporary table if exists  `ProssimaPartenza`;
	create temporary table  `ProssimaPartenza` (
		 `Codice_Tratta` char(5),
         `Veicolo_Associato` char(4),
         `Prima_Fermata` char(5),
         `Ultima_Fermata`  char(5),
		 `Data` date,
		 `Ora` time
	);

    set transaction isolation level read committed;
    start transaction;
    
		open cur;
		read_loop: loop
	
			fetch cur into var_TrattaIterazione, var_DataPartenza, var_OrarioPartenza ;
            if done then
				leave read_loop;
			end if;
             set var_TimeConfronto =  addtime(cast(var_DataPartenza as datetime),var_OrarioPartenza);
			if (to_seconds(var_TimeConfronto)-to_seconds(now()))>=0 then
				if var_PrimaIterazione then
					set var_PrimaIterazione = false;
					set var_mindiff =(to_seconds(var_TimeConfronto)-to_seconds(now()));
				end if;
				if (to_seconds(var_TimeConfronto)-to_seconds(now()))<=var_mindiff  then
					set var_mindiff =(to_seconds(var_TimeConfronto)-to_seconds(now()));
					set var_ProssimaPartenza_Data = var_DataPartenza;
					set var_ProssimaPartenza_Ora = var_OrarioPartenza;
					set var_TrattaStradale = var_TrattaIterazione;
				end if;
			end if;
		end loop;
        close cur;
        
        if var_TrattaStradale is null then
			signal sqlstate '45002' set message_text='Non esiste una partenza prevista per quel veicolo';
		end if;
        
	
        insert into `ProssimaPartenza`  
        select `t`.`CodiceIdentificativo`, `tr`.`VeicoloPubblico_Matricola`, `t`.`PrimaFermata`, `t`.`UltimaFermata`, `tr`.`DataPartenza`,`tr`.`OrarioPartenza`
        from `TrattaStradale` as `t` join `TrattaReale` as `tr` on `t`.`CodiceIdentificativo`= `tr`.`TrattaStradale`
        where `t`.`CodiceIdentificativo`=var_TrattaStradale and `tr`.`DataPartenza` = var_ProssimaPartenza_Data and `tr`.`OrarioPartenza` = var_ProssimaPartenza_Ora;
        
		select * from `ProssimaPartenza`;
        commit;
        drop temporary table  `ProssimaPartenza`;
END