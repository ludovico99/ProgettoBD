CREATE PROCEDURE ProssimaPartenzaPrevista (in var_matricola char(4))
BEGIN

    declare var_ProssimaPartenza_Data date default null;
    declare var_ProssimaPartenza_Ora time default null;
	declare var_ProssimaPartenza_Datetime datetime default null;
	declare var_TrattaStradale char(5) default null;
    

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

    set transaction isolation level serializable;
    start transaction;
    
		drop view if exists `Tratte_Datetime`;
		create view `Tratte_Datetime`(`TrattaStradale`, `Datetime`, `Veicolo Pubblico` ) 
		as select  `TrattaStradale`, addtime(cast(`DataPartenza` as datetime),`OrarioPartenza`), `VeicoloPubblico_Matricola`
        from  `TrattaReale` ;
		
        
		select `TrattaStradale`,  min(`Datetime`)
		from  `Tratte_Datetime` 
		where `Veicolo Pubblico`= var_matricola and `Datetime` >= now()
		group by `TrattaStradale`
		having  min(`Datetime`) into var_TrattaStradale, var_ProssimaPartenza_Datetime;
		
			
        
        if var_TrattaStradale is null then
			signal sqlstate '45002' set message_text='Non esiste una partenza prevista per quel veicolo';
		end if;
        
        set var_ProssimaPartenza_Data = date(var_ProssimaPartenza_Datetime);
		set var_ProssimaPartenza_Ora = time(var_ProssimaPartenza_Datetime);
        
        insert into `ProssimaPartenza`  
        select `t`.`CodiceIdentificativo`, `tr`.`VeicoloPubblico_Matricola`, `t`.`PrimaFermata`, `t`.`UltimaFermata`, `tr`.`DataPartenza`,`tr`.`OrarioPartenza`
        from `TrattaStradale` as `t` join `TrattaReale` as `tr` on `t`.`CodiceIdentificativo`= `tr`.`TrattaStradale`
        where `t`.`CodiceIdentificativo`=var_TrattaStradale and `tr`.`DataPartenza` = var_ProssimaPartenza_Data and `tr`.`OrarioPartenza` = var_ProssimaPartenza_Ora;
        
		select * from `ProssimaPartenza`;
        commit;
        drop temporary table  `ProssimaPartenza`;
END