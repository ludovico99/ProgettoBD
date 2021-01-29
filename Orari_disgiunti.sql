CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`OrariDisgiunti` BEFORE INSERT ON `TurniLavorativi`  FOR EACH ROW
BEGIN
	declare done int default false; 
    declare var_inizioTurno datetime;
	declare var_fineTurno datetime;
	declare cur cursor for select `InizioTurno`,`FineTurno`
							from `TurniLavorativi`
                            where `Conducente_CF`=new.Conducente_CF;
	declare continue handler for not found set done= true;      
    
     open cur;
     read_loop:loop
		fetch cur into var_inizioTurno, var_fineTurno;
        if (to_seconds(new.InizioTurno) - to_seconds(var_fineTurno))<0  then
			if (to_seconds(new.FineTurno) - to_seconds(var_inizioTurno))>0 then
			signal sqlstate '45014' set message_text = 'Turno di lavoro non disgiunto con quelli già presenti';
			end if;
		end if;
        if done then 
         leave read_loop;
         end if;
	end loop;
    close cur;
	
    
END