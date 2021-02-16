CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`AggregazioneTratta_Waypoint_AFTER_UPDATE` AFTER UPDATE ON `AggregazioneTratta_Waypoint` FOR EACH ROW
BEGIN
	declare var_codiceFermata char(5);
    declare last_waypoint int;
	if new.`Ordine` = 1 then
		select `CodiceFermata` from `Fermata` where `Waypoint_Latitudine`= new.`Waypoint_Latitudine` and `Waypoint_Longitudine`= new.`Waypoint_Longitudine` into var_codiceFermata;
        update `TrattaStradale` set `PrimaFermata` = var_codiceFermata
        where `CodiceIdentificativo` =new.`TrattaStradale_CodiceIdentificativo`;
	else 
		select min(`Ordine`) from `AggregazioneTratta_Waypoint`
		where `TrattaStradale_CodiceIdentificativo` = new.`TrattaStradale_CodiceIdentificativo`
		group by `TrattaStradale_CodiceIdentificativo` into last_waypoint;
        
        if last_waypoint = new.`Ordine` then
			select `CodiceFermata` from `Fermata` where `Waypoint_Latitudine`= new.`Waypoint_Latitudine` and `Waypoint_Longitudine`= new.`Waypoint_Longitudine` into var_codiceFermata;
			update `TrattaStradale` set `UltimaFermata` = var_codiceFermata
			where `CodiceIdentificativo` =new.`TrattaStradale_CodiceIdentificativo`;
		end if;
	end if;
END