CREATE PROCEDURE `Cerca_ConducentiValidiperLaSostituzione` (in var_Conducente char(16), in var_InizioTurno datetime, in var_FineTurno datetime)
BEGIN	

declare counter int;
declare var_Esiste int;

declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;
    
    
	set transaction read only;
    set transaction isolation level repeatable read;
	start transaction;
    

    select count(*) from `TurniLavorativi` where `Conducente_CF`= var_Conducente and `InizioTurno`=var_InizioTurno and `FineTurno`=var_FineTurno into var_Esiste;
    
    if var_Esiste = 0 then
		signal sqlstate '45004' set message_text='Il Conducente inserito non esegue quel turno';
    end if;
    
    select `CF`
	from `Conducente`
	where `InMalattia`=b'0'
	and `CF` not in (select `Conducente_CF` 
						from `TurniLavorativi`)
                
	union 
    
    
	select distinct `Conducente_CF`
	from `Conducente` left join `TurniLavorativi`  on  `CF` = `Conducente_CF`
	where  `Conducente_CF`  not in (select `Conducente_CF`
								from `TurniLavorativi` 
								where `InizioTurno` = var_InizioTurno and 
								`FineTurno`	= var_FineTurno	)
		
	and  `Conducente_CF` not in (select `Conducente_CF`
							  from `TurniLavorativi`  
							  group by `Conducente_CF`
							  having count(*)>4)
	and `InMalattia` =b'0'
    and `Conducente_CF` <> var_Conducente
    and `VeicoloPubblico_Matricola` in (select `VeicoloPubblico_Matricola` 
											from `Conducente` 
                                            where `CF` =var_Conducente)
	and valid_conducente(`Conducente_CF`, var_InizioTurno, var_FineTurno);
    
   
    select found_rows() into counter; 
	if counter = 0 then
		signal sqlstate '45004' set message_text = "Non esiste un conducente valido per la sostituzione";
	end if;
    
    commit;
END