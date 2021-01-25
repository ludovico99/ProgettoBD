-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema progetto
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `progetto` ;

-- -----------------------------------------------------
-- Schema progetto
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `progetto` DEFAULT CHARACTER SET utf8 ;
USE `progetto` ;

-- -----------------------------------------------------
-- Table `progetto`.`VeicoloPubblico`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`VeicoloPubblico` ;

CREATE TABLE IF NOT EXISTS `progetto`.`VeicoloPubblico` (
  `Matricola` CHAR(4) NOT NULL,
  `DataDiAcquisto` DATE NOT NULL,
  `Latitudine` FLOAT NULL,
  `Longitudine` FLOAT NULL,
  PRIMARY KEY (`Matricola`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `progetto`.`Waypoint`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`Waypoint` ;

CREATE TABLE IF NOT EXISTS `progetto`.`Waypoint` (
  `Latitudine` FLOAT NOT NULL,
  `Longitudine` FLOAT NOT NULL,
  PRIMARY KEY (`Latitudine`, `Longitudine`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `progetto`.`Fermata`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`Fermata` ;

CREATE TABLE IF NOT EXISTS `progetto`.`Fermata` (
  `CodiceFermata` CHAR(5) NOT NULL,
  `Waypoint_Latitudine` FLOAT NOT NULL,
  `Waypoint_Longitudine` FLOAT NOT NULL,
  PRIMARY KEY (`CodiceFermata`),
  CONSTRAINT `fk_Fermata_Waypoint1`
    FOREIGN KEY (`Waypoint_Latitudine` , `Waypoint_Longitudine`)
    REFERENCES `progetto`.`Waypoint` (`Latitudine` , `Longitudine`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_Fermata_Waypoint1_idx` ON `progetto`.`Fermata` (`Waypoint_Latitudine` ASC, `Waypoint_Longitudine` ASC) VISIBLE;


-- -----------------------------------------------------
-- Table `progetto`.`TrattaStradale`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`TrattaStradale` ;

CREATE TABLE IF NOT EXISTS `progetto`.`TrattaStradale` (
  `CodiceIdentificativo` CHAR(5) NOT NULL,
  `PrimaFermata` CHAR(5) NOT NULL,
  `UltimaFermata` CHAR(5) NOT NULL,
  PRIMARY KEY (`CodiceIdentificativo`),
  CONSTRAINT `fk_TrattaStradale_Fermata1`
    FOREIGN KEY (`PrimaFermata`)
    REFERENCES `progetto`.`Fermata` (`CodiceFermata`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_TrattaStradale_Fermata2`
    FOREIGN KEY (`UltimaFermata`)
    REFERENCES `progetto`.`Fermata` (`CodiceFermata`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_TrattaStradale_Fermata1_idx` ON `progetto`.`TrattaStradale` (`PrimaFermata` ASC) VISIBLE;

CREATE INDEX `fk_TrattaStradale_Fermata2_idx` ON `progetto`.`TrattaStradale` (`UltimaFermata` ASC) VISIBLE;


-- -----------------------------------------------------
-- Table `progetto`.`Utenti`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`Utenti` ;

CREATE TABLE IF NOT EXISTS `progetto`.`Utenti` (
  `Username` VARCHAR(45) NOT NULL,
  `Password` CHAR(32) NOT NULL,
  `Ruolo` ENUM('AmministratoreServizio', 'Conducente', 'UtenteSistema') NOT NULL,
  PRIMARY KEY (`Username`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `progetto`.`Conducente`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`Conducente` ;

CREATE TABLE IF NOT EXISTS `progetto`.`Conducente` (
  `CF` CHAR(16) NOT NULL,
  `Utenti_Username` VARCHAR(45) NOT NULL,
  `Nome` VARCHAR(45) NOT NULL,
  `Cognome` VARCHAR(45) NOT NULL,
  `DataDiNascita` DATE NOT NULL,
  `LuogoDiNascita` VARCHAR(45) NOT NULL,
  `NumPatente` CHAR(10) NOT NULL,
  `DataScadenzaPatente` DATE NOT NULL,
  `InMalattia` BIT(1) NOT NULL DEFAULT b'0',
  `VeicoloPubblico_Matricola` CHAR(4) NULL,
  PRIMARY KEY (`CF`),
  CONSTRAINT `fk_Conducente_VeicoloPubblico1`
    FOREIGN KEY (`VeicoloPubblico_Matricola`)
    REFERENCES `progetto`.`VeicoloPubblico` (`Matricola`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Conducente_Utenti1`
    FOREIGN KEY (`Utenti_Username`)
    REFERENCES `progetto`.`Utenti` (`Username`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_Conducente_VeicoloPubblico1_idx` ON `progetto`.`Conducente` (`VeicoloPubblico_Matricola` ASC) VISIBLE;

CREATE INDEX `fk_Conducente_Utenti1_idx` ON `progetto`.`Conducente` (`Utenti_Username` ASC) VISIBLE;


-- -----------------------------------------------------
-- Table `progetto`.`Abbonamento`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`Abbonamento` ;

CREATE TABLE IF NOT EXISTS `progetto`.`Abbonamento` (
  `Codice` INT NOT NULL AUTO_INCREMENT,
  `UltimoUtilizzo` DATE NULL,
  `VeicoloPubblico_Matricola` CHAR(4) NULL,
  PRIMARY KEY (`Codice`),
  CONSTRAINT `fk_Abbonamento_VeicoloPubblico1`
    FOREIGN KEY (`VeicoloPubblico_Matricola`)
    REFERENCES `progetto`.`VeicoloPubblico` (`Matricola`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_Abbonamento_VeicoloPubblico1_idx` ON `progetto`.`Abbonamento` (`VeicoloPubblico_Matricola` ASC) VISIBLE;


-- -----------------------------------------------------
-- Table `progetto`.`BigliettoElettronico`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`BigliettoElettronico` ;

CREATE TABLE IF NOT EXISTS `progetto`.`BigliettoElettronico` (
  `CodiceBigliettoEmesso` INT NOT NULL AUTO_INCREMENT,
  `Utilizzato` BIT(1) NULL DEFAULT 0,
  `VeicoloPubblico_Matricola` CHAR(4) NULL,
  PRIMARY KEY (`CodiceBigliettoEmesso`),
  CONSTRAINT `fk_BigliettoElettronico_VeicoloPubblico1`
    FOREIGN KEY (`VeicoloPubblico_Matricola`)
    REFERENCES `progetto`.`VeicoloPubblico` (`Matricola`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_BigliettoElettronico_VeicoloPubblico1_idx` ON `progetto`.`BigliettoElettronico` (`VeicoloPubblico_Matricola` ASC) INVISIBLE;


-- -----------------------------------------------------
-- Table `progetto`.`TurniLavorativi`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`TurniLavorativi` ;

CREATE TABLE IF NOT EXISTS `progetto`.`TurniLavorativi` (
  `Conducente_CF` CHAR(16) NOT NULL,
  `InizioTurno` DATETIME NOT NULL,
  `FineTurno` DATETIME NOT NULL,
  PRIMARY KEY (`Conducente_CF`, `InizioTurno`, `FineTurno`),
  CONSTRAINT `fk_TurniLavorativi_Conducente1`
    FOREIGN KEY (`Conducente_CF`)
    REFERENCES `progetto`.`Conducente` (`CF`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_TurniLavorativi_Conducente1_idx` ON `progetto`.`TurniLavorativi` (`Conducente_CF` ASC) VISIBLE;


-- -----------------------------------------------------
-- Table `progetto`.`TrattaReale`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`TrattaReale` ;

CREATE TABLE IF NOT EXISTS `progetto`.`TrattaReale` (
  `TrattaStradale` CHAR(5) NOT NULL,
  `DataPartenza` DATE NOT NULL,
  `OrarioPartenza` TIME NOT NULL,
  `VeicoloPubblico_Matricola` CHAR(4) NOT NULL,
  PRIMARY KEY (`TrattaStradale`, `DataPartenza`, `OrarioPartenza`),
  CONSTRAINT `fk_TrattaReale_VeicoloPubblico1`
    FOREIGN KEY (`VeicoloPubblico_Matricola`)
    REFERENCES `progetto`.`VeicoloPubblico` (`Matricola`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_TrattaReale_TrattaStradale1`
    FOREIGN KEY (`TrattaStradale`)
    REFERENCES `progetto`.`TrattaStradale` (`CodiceIdentificativo`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_TrattaReale_VeicoloPubblico1_idx` ON `progetto`.`TrattaReale` (`VeicoloPubblico_Matricola` ASC) VISIBLE;

CREATE INDEX `fk_TrattaReale_TrattaStradale1_idx` ON `progetto`.`TrattaReale` (`TrattaStradale` ASC) VISIBLE;


-- -----------------------------------------------------
-- Table `progetto`.`Manutenzione`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`Manutenzione` ;

CREATE TABLE IF NOT EXISTS `progetto`.`Manutenzione` (
  `VeicoloPubblico_Matricola` CHAR(4) NOT NULL,
  `Data` DATE NOT NULL,
  `Costo` FLOAT NOT NULL,
  `TipoIntervento` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`VeicoloPubblico_Matricola`, `Data`),
  CONSTRAINT `fk_Manutenzione_VeicoloPubblico1`
    FOREIGN KEY (`VeicoloPubblico_Matricola`)
    REFERENCES `progetto`.`VeicoloPubblico` (`Matricola`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_Manutenzione_VeicoloPubblico1_idx` ON `progetto`.`Manutenzione` (`VeicoloPubblico_Matricola` ASC) VISIBLE;


-- -----------------------------------------------------
-- Table `progetto`.`AggregazioneTratta_Waypoint`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `progetto`.`AggregazioneTratta_Waypoint` ;

CREATE TABLE IF NOT EXISTS `progetto`.`AggregazioneTratta_Waypoint` (
  `TrattaStradale_CodiceIdentificativo` CHAR(5) NOT NULL,
  `Waypoint_Latitudine` FLOAT NOT NULL,
  `Waypoint_Longitudine` FLOAT NOT NULL,
  `Ordine` INT NOT NULL,
  PRIMARY KEY (`TrattaStradale_CodiceIdentificativo`, `Waypoint_Latitudine`, `Waypoint_Longitudine`),
  CONSTRAINT `fk_AggregazioneTratta_Waypoint_TrattaStradale1`
    FOREIGN KEY (`TrattaStradale_CodiceIdentificativo`)
    REFERENCES `progetto`.`TrattaStradale` (`CodiceIdentificativo`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_AggregazioneTratta_Waypoint_Waypoint1`
    FOREIGN KEY (`Waypoint_Latitudine` , `Waypoint_Longitudine`)
    REFERENCES `progetto`.`Waypoint` (`Latitudine` , `Longitudine`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_AggregazioneTratta_Waypoint_TrattaStradale1_idx` ON `progetto`.`AggregazioneTratta_Waypoint` (`TrattaStradale_CodiceIdentificativo` ASC) INVISIBLE;

CREATE INDEX `fk_AggregazioneTratta_Waypoint_Waypoint1_idx` ON `progetto`.`AggregazioneTratta_Waypoint` (`Waypoint_Latitudine` ASC, `Waypoint_Longitudine` ASC) VISIBLE;

USE `progetto` ;

-- -----------------------------------------------------
-- procedure AssociazioneVeicoli_TrattaReale
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`AssociazioneVeicoli_TrattaReale`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `AssociazioneVeicoli_TrattaReale` (in var_codiceTratta char(5), in var_veicoloPubblico char(4),in var_orarioPartenza time, in var_Data date)
BEGIN 
declare counter integer;

declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;

set transaction isolation level serializable;
start transaction;
set counter=0;

select count(*)
from `TrattaReale`  as `tr`
where `tr`.`TrattaStradale` = var_codiceTratta and `tr`.`DataPartenza`=var_Data and `tr`.`OrarioPartenza` = var_orarioPartenza into counter;

if counter=0 then
	insert into `TrattaReale`(`TrattaStradale`, `DataPartenza `, `OrarioPartenza`, `VeicoloPubblico_Matricola`) values (var_codiceTratta, var_Data, var_orarioPartenza, var_veicoloPubblico);
else  
	
	update `TrattaReale` set `VeicoloPubblico_Matricola`= var_veicoloPubblico 
    where `TrattaStradale`=var_codiceTratta and `DataPartenza`= var_Data and  `OrarioPartenza` = var_orarioPartenza;
    
END if;
commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Aggiungi_NuovoTurno_Conducente
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Aggiungi_NuovoTurno_Conducente`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Aggiungi_NuovoTurno_Conducente` (in var_Conducente char(16) , in var_inizioTurno datetime, in var_fineTurno datetime)
BEGIN
	declare risultato char(16) default null;
    declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;
    
    set transaction isolation level read committed;
	start transaction;
    
	select `CF` from `Conducente` where `CF`=var_Conducente into risultato;
    if risultato is null then
		signal sqlstate '45004' set message_text = 'Il conducente inserito non è valido';
	end if;
	insert into `TurniLavorativi`  (`Conducente_CF`, `InizioTurno`, `FineTurno`) values (var_Conducente, var_inizioTurno , var_fineTurno);
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure EliminareOrarioConducente
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`EliminareOrarioConducente`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `EliminareOrarioConducente` (in var_Conducente char(16) , in var_inizioTurno datetime, in var_fineTurno datetime)
BEGIN
	Delete from `TurniLavorativi`  where `Conducente_CF`= var_Conducente and `InizioTurno`= var_inizioTurno and `FineTurno` = var_fineTurno;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure SostituzioneTurno
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`SostituzioneTurno`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `SostituzioneTurno` (in var_NuovoConducente char(16), in var_ConducenteSostituito char(16), in var_InizioTurno datetime, in var_FineTurno datetime)
BEGIN 	
declare var_VeicoloPubblico char(4);
declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;

set transaction isolation level read committed;
  start transaction;

	update `Conducente`  set `InMalattia` = b'1'
	where `CF` = var_ConducenteSostituito ;
    
    select `VeicoloPubblico_Matricola`
	from `Conducente` 
	where `CF` = var_ConducenteSostituito into var_VeicoloPubblico;
    
	update `Conducente`  set `VeicoloPubblico_Matricola` = var_VeicoloPubblico
    where  `CF`= var_NuovoConducente;
    
	update `TurniLavorativi`  set `Conducente_CF` = var_NuovoConducente
	where `Conducente_CF` = var_ConducenteSostituito and `InizioTurno`=var_InizioTurno and `FineTurno`=var_FineTurno;
	
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure EmissioneBiglietto
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`EmissioneBiglietto`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `EmissioneBiglietto` (Out var_CodiceBiglietto int)
BEGIN
		insert into `BigliettoElettronico` (`Utilizzato`,`VeicoloPubblico_Matricola`) values (b'0', Null);
        
        set var_CodiceBiglietto = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure EmissioneAbbonamento
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`EmissioneAbbonamento`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `EmissioneAbbonamento` (out var_CodiceAbbonamento int)
BEGIN
	insert into `Abbonamento`(`UltimoUtilizzo`,`VeicoloPubblico_Matricola`) values (null, null);
    
    set var_CodiceAbbonamento = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure ProssimaPartenzaPrevista
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`ProssimaPartenzaPrevista`;

DELIMITER $$
USE `progetto`$$
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
	declare cur cursor for select  `TrattaStradale`, `DataPartenza`,`OrarioPartenza` from  `TrattaReale` where `VeicoloPubblico_Matricola`= var_matricola and (`DataPartenza` = curdate() or `DataPartenza` = date_add(curdate(),interval 1 day));
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
			if time_to_sec(timediff(var_TimeConfronto,now()))>=0 then
				if var_PrimaIterazione then
					set var_PrimaIterazione = false;
					set var_mindiff =time_to_sec(timediff(var_TimeConfronto,now()));
				end if;
				if time_to_sec(timediff(var_TimeConfronto,now()))<=var_mindiff  then
					set var_mindiff =time_to_sec(timediff(var_TimeConfronto,now()));
					set var_ProssimaPartenza_Data = var_DataPartenza;
					set var_ProssimaPartenza_Ora = var_OrarioPartenza;
					set var_TrattaStradale = var_TrattaIterazione;
				end if;
			end if;
		end loop;
        close cur;
        
        if var_TrattaStradale is null then
			signal sqlstate '45002' set message_text='Non esiste una partenza prevista entro 1 giorno';
		end if;
        
	
        insert into `ProssimaPartenza`  
        select `t`.`CodiceIdentificativo`, `tr`.`VeicoloPubblico_Matricola`, `t`.`PrimaFermata`, `t`.`UltimaFermata`, `tr`.`DataPartenza`,`tr`.`OrarioPartenza`
        from `TrattaStradale` as `t` join `TrattaReale` as `tr` on `t`.`CodiceIdentificativo`= `tr`.`TrattaStradale`
        where `t`.`CodiceIdentificativo`=var_TrattaStradale and `tr`.`DataPartenza` = var_ProssimaPartenza_Data and `tr`.`OrarioPartenza` = var_ProssimaPartenza_Ora;
            
		select * from `ProssimaPartenza`;
        commit;
        drop temporary table  `ProssimaPartenza`;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Biglietto_Utilizzato
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Biglietto_Utilizzato`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Biglietto_Utilizzato` (in var_Biglietto int, in var_VeicoloValidante char(4))
BEGIN
	declare done int default false;
	declare var_BigliettoEsistente int;
	declare cur cursor for select `CodiceBigliettoEmesso` from `BigliettoElettronico`;
                              
declare continue handler for not found set done= true;
declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;

set transaction isolation level read committed;
  start transaction;
   
    open cur;
    read_loop: loop
		fetch cur into var_BigliettoEsistente;
		if var_BigliettoEsistente =var_Biglietto then
			leave read_loop;
        end if;
        if done then
			signal sqlstate '45001' set message_text='Il Biglietto inserito non è valido';
		end if;
	end loop;
    close cur;
    
	update `BigliettoElettronico` set `Utilizzato`=b'1', `VeicoloPubblico_Matricola`=var_VeicoloValidante 
    where `CodiceBigliettoEmesso` = var_Biglietto;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Abbonamento_Utilizzato
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Abbonamento_Utilizzato`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Abbonamento_Utilizzato` (in var_CodiceBiglietto int, in var_UltimoUtilizzo date, in var_VeicoloValidante char(4))
BEGIN
	declare done int default false;
	declare var_AbbonamentoEsistente int;
	declare cur cursor for select Codice from Abbonamento;
                              
declare continue handler for not found set done= true;
declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;

set transaction isolation level read committed;
  start transaction;
   
    open cur;
    read_loop: loop
		fetch cur into var_AbbonamentoEsistente;
		if var_AbbonamentoEsistente =var_CodiceBiglietto then
			leave read_loop;
        end if;
        if done then
			signal sqlstate '45000' set message_text='Abbonamento inserito non è valido';
		end if;
	end loop;
    close cur;
    
	update `Abbonamento` set `UltimoUtilizzo`=var_UltimoUtilizzo, `VeicoloPubblico_Matricola` =var_VeicoloValidante 
    where `Codice` = var_CodiceBiglietto;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure ConducenteGuarito
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`ConducenteGuarito`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `ConducenteGuarito` (in var_Conducente char(16))
BEGIN
	update `Conducente` set `InMalattia` = b'0'
	where `CF` = var_Conducente ;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Cerca_ConducentiValidiperLaSostituzione
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Cerca_ConducentiValidiperLaSostituzione`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Cerca_ConducentiValidiperLaSostituzione` (in var_Conducente char(16), in var_InizioTurno datetime, in var_FineTurno datetime)
BEGIN	
declare var_Esiste char(16);
declare done int;
declare cur cursor for select `Conducente_CF` from `TurniLavorativi` where `Conducente_CF`= var_Conducente and `InizioTurno`=var_InizioTurno and `FineTurno`=var_FineTurno ;
declare continue handler for not found set done= true;
declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;
    
    
	set transaction read only;
    set transaction isolation level read committed;
	start transaction;
    
    open cur;
    read_loop: loop
		fetch cur into var_Esiste;
		if var_Esiste is not null then
			leave read_loop;
        end if;
        if done then
			signal sqlstate '45003' set message_text='Il Conducente inserito non esegue quel turno';
		end if;
	end loop;
    close cur;
    
	select `CF`
	from `Conducente`
	where `InMalattia`=b'0'
	and `CF` not in (select `Conducente_CF` 
						from `TurniLavorativi`)
                
	union 

	select distinct `Conducente_CF`
	from `Conducente` join `TurniLavorativi`  on  `CF` = `Conducente_CF`
	where  `Conducente_CF`  not in (select `Conducente_CF`
								from `TurniLavorativi` 
								where `InizioTurno` = var_InizioTurno and 
								`FineTurno`	= var_FineTurno	)
		
	and  `Conducente_CF` not in (select `Conducente_CF`
							  from `TurniLavorativi`  
							  group by `Conducente_CF`
							  having count(*)=5)
	and `InMalattia` =b'0'
    and `Conducente_CF` <> var_Conducente
    and `VeicoloPubblico_Matricola` in (select `VeicoloPubblico_Matricola` 
											from `Conducente` 
                                            where `CF` =var_Conducente)
	and valid_conducente(`Conducente_CF`, var_InizioTurno, var_FineTurno);
		
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Distanza_Veicolo_Fermata
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Distanza_Veicolo_Fermata`;

DELIMITER $$
USE `progetto`$$
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
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Login
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Login`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Login` (in var_username varchar(45), in var_pass varchar(45), out var_role INT)
BEGIN
	declare var_user_role ENUM('AmministratoreServizio', 'Conducente', 'UtenteSistema');
    
	select `Ruolo` from `Utenti`
		where `Username` = var_username
        and `Password` = md5(var_pass)
        into var_user_role;
        
        -- See the corresponding enum in the client
		if var_user_role = 'AmministratoreServizio' then
			set var_role = 1;
		elseif var_user_role = 'Conducente' then
			set var_role = 2;
		elseif var_user_role = 'UtenteSistema' then
			set var_role = 3;
		else
			set var_role = 4;
		end if;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure InformazioniGPS
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`InformazioniGPS`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `InformazioniGPS` (in var_VeicoloPubblico char(4), in var_Latitudine float, in var_Longitudine float)
BEGIN
	update `VeicoloPubblico` set `Latitudine` = var_Latitudine, `Longitudine` = var_Longitudine
    where `Matricola` = var_VeicoloPubblico;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Crea_Utente
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Crea_Utente`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Crea_Utente` (IN var_username VARCHAR(45), IN var_password VARCHAR(45), IN var_ruolo varchar(45))
BEGIN 
	insert into `Utenti` VALUES(var_username, MD5(var_password), var_ruolo);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Aggiungi_Conducente
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Aggiungi_Conducente`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Aggiungi_Conducente` (IN var_CF char(16), IN var_Username varchar(45), IN var_Nome VARCHAR(45),  IN var_Cognome VARCHAR(45),  IN var_DataDiNascita DATE, IN var_LuogoDiNascita  VARCHAR(45), IN var_NumeroPatente CHAR(10), IN var_ScadenzaPatente DATE, IN var_VeicoloAssegnato CHAR(4))
BEGIN
	declare var_Ruolo enum('AmministratoreServizio', 'Conducente', 'UtenteSistema');
    
	declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;

    set transaction isolation level read committed;
    start transaction;
		
        
        select `Ruolo` from `Utenti` where `Username` = var_Username into var_Ruolo;
        
        if var_ruolo <> 'Conducente' then
			signal sqlstate '45007' set message_text = "Il Conducente che si vuole inserire non è tale";
		end if;
        
        insert into `Conducente` values (var_CF, var_Username, var_Nome, var_Cognome, var_DataDiNascita, var_LuogoDiNascita, var_NumeroPatente, var_ScadenzaPatente, b'0', var_VeicoloAssegnato);
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Aggiungi_TrattaStradale
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Aggiungi_TrattaStradale`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Aggiungi_TrattaStradale` (IN var_Codice CHAR(5), IN var_PrimaFermata char(5), var_UltimaFermata char(5))
BEGIN
	insert into `TrattaStradale` values (var_Codice, var_PrimaFermata, var_UltimaFermata);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Aggiungi_TrattaReale
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Aggiungi_TrattaReale`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Aggiungi_TrattaReale` (in var_TrattaStradale char(5), in var_DataPartenza date, in var_OrarioPartenza time, in var_Veicolo char(4))
BEGIN
	 insert into `TrattaReale` values (var_TrattaStradale, var_DataPartenza, var_OrarioPartenza, var_Veicolo);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Aggiungi_Waypoint
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Aggiungi_Waypoint`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Aggiungi_Waypoint` (in var_Latitudine float, in var_Longitudine float)
BEGIN
	insert into `Waypoint` values (var_Latitudine, var_Longitudine);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Aggiungi_Fermata
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Aggiungi_Fermata`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Aggiungi_Fermata` (in var_CodiceFermata char(5), in var_Latitudine float, in var_Longitudine float)
BEGIN
	insert into `Fermata` values (var_CodiceFermata, var_Latitudine, var_Longitudine);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Aggiungi_Manutenzione
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Aggiungi_Manutenzione`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Aggiungi_Manutenzione` (in var_Matricola char(4), in var_Data date, in var_Costo float, in var_TipoIntervento varchar(45))
BEGIN
	insert into `Manutenzione` values (var_Matricola, var_Data, var_Costo, var_TipoIntervento); 
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Aggiungi_VeicoloPubblico
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Aggiungi_VeicoloPubblico`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Aggiungi_VeicoloPubblico` (in var_Matricola char (4), in var_DataAcquisto date)
BEGIN
	insert into `VeicoloPubblico` values (var_Matricola, var_DataAcquisto, null, null);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure Aggiungi_Waypoint_per_una_tratta
-- -----------------------------------------------------

USE `progetto`;
DROP procedure IF EXISTS `progetto`.`Aggiungi_Waypoint_per_una_tratta`;

DELIMITER $$
USE `progetto`$$
CREATE PROCEDURE `Aggiungi_Waypoint_per_una_tratta` (in var_Tratta char(5), in var_Latitudine float, in var_Longitudine float, in var_NumeroOrdine int)
BEGIN
	declare var_NumeroOrdineIterazione int default null;
	declare done int default false;
	declare cur cursor for select `Ordine` from `AggregazioneTratta_Waypoint` where `TrattaStradale_CodiceIdentificativo` = var_Tratta and `Ordine` >= var_NumeroOrdine order by `Ordine` desc;
    declare continue handler for not found set done = true;
    
   declare exit handler for sqlexception
    begin
        rollback;  -- rollback any changes made in the transaction
        resignal;  -- raise again the sql exception to the caller
    end;
    
    set transaction isolation level repeatable read;
    start transaction;
    
		select `Ordine` from `AggregazioneTratta_Waypoint` where `TrattaStradale_CodiceIdentificativo` = var_Tratta  and `Ordine` = var_NumeroOrdine into var_NumeroOrdineIterazione;
    
		if var_NumeroOrdineIterazione is not null then 
			set var_NumeroOrdineIterazione = 0;
			open cur;
			read_loop: loop
				fetch cur into var_NumeroOrdineIterazione;
					if done then
						leave read_loop;
					end if;
					update `AggregazioneTratta_Waypoint` set `Ordine` = (var_NumeroOrdineIterazione + 1)
					where `TrattaStradale_CodiceIdentificativo` = var_Tratta  and `Ordine` = var_NumeroOrdineIterazione;
			end loop;
			close cur;
        end if;
		
     insert into `AggregazioneTratta_Waypoint` values (var_Tratta, var_latitudine, var_Longitudine, var_NumeroOrdine);
	 commit;
END$$

DELIMITER ;
SET SQL_MODE = '';
DROP USER IF EXISTS AmministratoreServizio;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'AmministratoreServizio' IDENTIFIED BY 'amministratoreservizio';

GRANT EXECUTE ON procedure `progetto`.`AssociazioneVeicoli_TrattaReale` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Aggiungi_NuovoTurno_Conducente` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`EliminareOrarioConducente` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`SostituzioneTurno` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`EmissioneBiglietto` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`EmissioneAbbonamento` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`ConducenteGuarito` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Cerca_ConducentiValidiperLaSostituzione` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Aggiungi_Conducente` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Aggiungi_Fermata` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Aggiungi_Manutenzione` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Aggiungi_TrattaReale` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Aggiungi_TrattaStradale` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Aggiungi_VeicoloPubblico` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Aggiungi_Waypoint` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Aggiungi_Waypoint_per_una_tratta` TO 'AmministratoreServizio';
GRANT EXECUTE ON procedure `progetto`.`Crea_Utente` TO 'AmministratoreServizio';
SET SQL_MODE = '';
DROP USER IF EXISTS Conducente;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'Conducente' IDENTIFIED BY 'conducente';

GRANT EXECUTE ON procedure `progetto`.`ProssimaPartenzaPrevista` TO 'Conducente';
SET SQL_MODE = '';
DROP USER IF EXISTS UtenteSistema;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'UtenteSistema' IDENTIFIED BY 'utentesistema';

GRANT EXECUTE ON procedure `progetto`.`Distanza_Veicolo_Fermata` TO 'UtenteSistema';
SET SQL_MODE = '';
DROP USER IF EXISTS Login;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'Login' IDENTIFIED BY 'login';

GRANT EXECUTE ON procedure `progetto`.`Login` TO 'Login';
SET SQL_MODE = '';
DROP USER IF EXISTS ValidatoreElettronico;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'ValidatoreElettronico' IDENTIFIED BY 'pippo';

GRANT EXECUTE ON procedure `progetto`.`Biglietto_Utilizzato` TO 'ValidatoreElettronico';
GRANT EXECUTE ON procedure `progetto`.`Abbonamento_Utilizzato` TO 'ValidatoreElettronico';
SET SQL_MODE = '';
DROP USER IF EXISTS GPS;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'GPS' IDENTIFIED BY 'pippo';

GRANT EXECUTE ON procedure `progetto`.`InformazioniGPS` TO 'GPS';

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `progetto`.`VeicoloPubblico`
-- -----------------------------------------------------
START TRANSACTION;
USE `progetto`;
INSERT INTO `progetto`.`VeicoloPubblico` (`Matricola`, `DataDiAcquisto`, `Latitudine`, `Longitudine`) VALUES ('1234', '2018-10-09', 135.69, 140.27);
INSERT INTO `progetto`.`VeicoloPubblico` (`Matricola`, `DataDiAcquisto`, `Latitudine`, `Longitudine`) VALUES ('1111', '2018-10-09', 1, 1);

COMMIT;


-- -----------------------------------------------------
-- Data for table `progetto`.`Waypoint`
-- -----------------------------------------------------
START TRANSACTION;
USE `progetto`;
INSERT INTO `progetto`.`Waypoint` (`Latitudine`, `Longitudine`) VALUES (128.2432, 40.97);
INSERT INTO `progetto`.`Waypoint` (`Latitudine`, `Longitudine`) VALUES (128.300, 40.100);

COMMIT;


-- -----------------------------------------------------
-- Data for table `progetto`.`Fermata`
-- -----------------------------------------------------
START TRANSACTION;
USE `progetto`;
INSERT INTO `progetto`.`Fermata` (`CodiceFermata`, `Waypoint_Latitudine`, `Waypoint_Longitudine`) VALUES ('12346', 128.2432, 40.97);
INSERT INTO `progetto`.`Fermata` (`CodiceFermata`, `Waypoint_Latitudine`, `Waypoint_Longitudine`) VALUES ('12347', 128.300, 40.100);

COMMIT;


-- -----------------------------------------------------
-- Data for table `progetto`.`TrattaStradale`
-- -----------------------------------------------------
START TRANSACTION;
USE `progetto`;
INSERT INTO `progetto`.`TrattaStradale` (`CodiceIdentificativo`, `PrimaFermata`, `UltimaFermata`) VALUES ('12345', '12346', '12347');

COMMIT;


-- -----------------------------------------------------
-- Data for table `progetto`.`Utenti`
-- -----------------------------------------------------
START TRANSACTION;
USE `progetto`;
INSERT INTO `progetto`.`Utenti` (`Username`, `Password`, `Ruolo`) VALUES ('Mario', 'c4ca4238a0b923820dcc509a6f75849b', 'Conducente');
INSERT INTO `progetto`.`Utenti` (`Username`, `Password`, `Ruolo`) VALUES ('Giuseppe', 'c81e728d9d4c2f636f067f89cc14862c', 'AmministratoreServizio');
INSERT INTO `progetto`.`Utenti` (`Username`, `Password`, `Ruolo`) VALUES ('Francesco', 'eccbc87e4b5ce2fe28308fd9f2a7baf3', 'Conducente');
INSERT INTO `progetto`.`Utenti` (`Username`, `Password`, `Ruolo`) VALUES ('Alessandro', 'a87ff679a2f3e71d9181a67b7542122c', 'Conducente');

COMMIT;


-- -----------------------------------------------------
-- Data for table `progetto`.`Conducente`
-- -----------------------------------------------------
START TRANSACTION;
USE `progetto`;
INSERT INTO `progetto`.`Conducente` (`CF`, `Utenti_Username`, `Nome`, `Cognome`, `DataDiNascita`, `LuogoDiNascita`, `NumPatente`, `DataScadenzaPatente`, `InMalattia`, `VeicoloPubblico_Matricola`) VALUES ('1234567891111111', 'Mario', 'pippo', 'pluto', '2020-12-11', 'Sora', '0123456789', '2020-12-09', 0, '1234');
INSERT INTO `progetto`.`Conducente` (`CF`, `Utenti_Username`, `Nome`, `Cognome`, `DataDiNascita`, `LuogoDiNascita`, `NumPatente`, `DataScadenzaPatente`, `InMalattia`, `VeicoloPubblico_Matricola`) VALUES ('1234567891111112', 'Francesco', 'pippo', 'pluto', '2020-12-11', 'Sora', '0123456789', '2020-12-09', 0, '1234');
INSERT INTO `progetto`.`Conducente` (`CF`, `Utenti_Username`, `Nome`, `Cognome`, `DataDiNascita`, `LuogoDiNascita`, `NumPatente`, `DataScadenzaPatente`, `InMalattia`, `VeicoloPubblico_Matricola`) VALUES ('1234567891111113', 'Alessandro', 'pippo', 'pluto', '2020-12-11', 'Sora', '0123456789', '2020-12-09', 0, NULL);

COMMIT;


-- -----------------------------------------------------
-- Data for table `progetto`.`TurniLavorativi`
-- -----------------------------------------------------
START TRANSACTION;
USE `progetto`;
INSERT INTO `progetto`.`TurniLavorativi` (`Conducente_CF`, `InizioTurno`, `FineTurno`) VALUES ('1234567891111111', '2020-12-10 8:30:00', '2020-12-10 11:30:00');
INSERT INTO `progetto`.`TurniLavorativi` (`Conducente_CF`, `InizioTurno`, `FineTurno`) VALUES ('1234567891111111', '2020-12-11 08:30:00', '2020-12-11 11:40');
INSERT INTO `progetto`.`TurniLavorativi` (`Conducente_CF`, `InizioTurno`, `FineTurno`) VALUES ('1234567891111111', '2020-12-12 08:30:00', '2020-12-12 11:45');
INSERT INTO `progetto`.`TurniLavorativi` (`Conducente_CF`, `InizioTurno`, `FineTurno`) VALUES ('1234567891111111', '2020-12-13 08:30:00', '2020-12-13 10:40');
INSERT INTO `progetto`.`TurniLavorativi` (`Conducente_CF`, `InizioTurno`, `FineTurno`) VALUES ('1234567891111111', '2020-12-14 08:30:00', '2020-12-14 9:45');

COMMIT;


-- -----------------------------------------------------
-- Data for table `progetto`.`TrattaReale`
-- -----------------------------------------------------
START TRANSACTION;
USE `progetto`;
INSERT INTO `progetto`.`TrattaReale` (`TrattaStradale`, `DataPartenza`, `OrarioPartenza`, `VeicoloPubblico_Matricola`) VALUES ('12345', '2020-09-10', '8:30', '1234');
INSERT INTO `progetto`.`TrattaReale` (`TrattaStradale`, `DataPartenza`, `OrarioPartenza`, `VeicoloPubblico_Matricola`) VALUES ('12345', '2021-01-17', '13:30:00', '1234');
INSERT INTO `progetto`.`TrattaReale` (`TrattaStradale`, `DataPartenza`, `OrarioPartenza`, `VeicoloPubblico_Matricola`) VALUES ('12345', '2021-01-18', '20:30:00', '1234');
INSERT INTO `progetto`.`TrattaReale` (`TrattaStradale`, `DataPartenza`, `OrarioPartenza`, `VeicoloPubblico_Matricola`) VALUES ('12345', '2021-01-18', '00:30:00', '1234');
INSERT INTO `progetto`.`TrattaReale` (`TrattaStradale`, `DataPartenza`, `OrarioPartenza`, `VeicoloPubblico_Matricola`) VALUES ('12345', '2021-01-17', '12:40:00', '1234');

COMMIT;


-- -----------------------------------------------------
-- Data for table `progetto`.`AggregazioneTratta_Waypoint`
-- -----------------------------------------------------
START TRANSACTION;
USE `progetto`;
INSERT INTO `progetto`.`AggregazioneTratta_Waypoint` (`TrattaStradale_CodiceIdentificativo`, `Waypoint_Latitudine`, `Waypoint_Longitudine`, `Ordine`) VALUES ('12345', 128.2432, 40.97, 2);
INSERT INTO `progetto`.`AggregazioneTratta_Waypoint` (`TrattaStradale_CodiceIdentificativo`, `Waypoint_Latitudine`, `Waypoint_Longitudine`, `Ordine`) VALUES ('12345', 128.300, 40.100, 1);

COMMIT;

USE `progetto`;

DELIMITER $$

USE `progetto`$$
DROP TRIGGER IF EXISTS `progetto`.`MatricolaDi4Cifre` $$
USE `progetto`$$
CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`MatricolaDi4Cifre` BEFORE INSERT ON `VeicoloPubblico` FOR EACH ROW
BEGIN
 if not (NEW.Matricola regexp '^[0-9]{4}$')  then
     signal sqlstate '45015' set message_text = 'La matricola inserita ha un numero di cifre diverso da 4 oppure contiene caratteri alfabetici';
     end if;
END$$


USE `progetto`$$
DROP TRIGGER IF EXISTS `progetto`.`CodiceFermataDi5Cifre` $$
USE `progetto`$$
CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`CodiceFermataDi5Cifre` BEFORE INSERT ON `Fermata` FOR EACH ROW
BEGIN

	if not (NEW.CodiceFermata regexp '^[0-9]{5}$*') then
     signal sqlstate '45016' set message_text = 'Il codice fermata inserito ha un numero di cifre diverso da 5 oppure contiene caratteri alfabetici';
     end if;
END$$


USE `progetto`$$
DROP TRIGGER IF EXISTS `progetto`.`CodiceTrattaDI5Cifre` $$
USE `progetto`$$
CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`CodiceTrattaDI5Cifre` BEFORE INSERT ON `TrattaStradale` FOR EACH ROW
BEGIN
	if not (NEW.CodiceIdentificativo regexp '^[0-9]{5}$') then
     signal sqlstate '45017' set message_text = 'Il codice di tratta inserito ha un numero di cifre diverso da 5 oppure contiene caratteri alfabetici';
     end if;
END$$


USE `progetto`$$
DROP TRIGGER IF EXISTS `progetto`.`InizioTurno_FineTurno` $$
USE `progetto`$$
CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`InizioTurno_FineTurno` BEFORE INSERT ON `TurniLavorativi`  FOR EACH ROW
BEGIN
  declare OreDifferenza time;
  set OreDifferenza=0;
  set OreDifferenza= timediff(NEW.fineturno,NEW.inizioturno);
  IF time_to_sec(OreDifferenza)<0 THEN 
  signal sqlstate '45010' set message_text ='Orario fine turno minore di quello iniziale';
  END IF;  
END$$


USE `progetto`$$
DROP TRIGGER IF EXISTS `progetto`.`max turni per conducente` $$
USE `progetto`$$
CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`max turni per conducente` BEFORE INSERT ON `TurniLavorativi`  FOR EACH ROW
BEGIN
	declare counter int; 
    set counter=0;
    select count(*)
    from `TurniLavorativi` 
    where `Conducente_CF` = NEW.conducente_CF into counter;
    
    if counter > 4 then
     signal sqlstate '45011' set message_text = 'troppi turni per quel conducente'; 
     end if;
END$$


USE `progetto`$$
DROP TRIGGER IF EXISTS `progetto`.`turniDi8Ore` $$
USE `progetto`$$
CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`turniDi8Ore` BEFORE INSERT ON `TurniLavorativi`  FOR EACH ROW
BEGIN
  declare OreDifferenza time;
  set OreDifferenza=0;
  set OreDifferenza= timediff(NEW.fineturno,NEW.inizioturno);
  IF time_to_sec(OreDifferenza)>28800 THEN 
  signal sqlstate '45012' set message_text =' Piu di 8 ore di differenza';
  END IF;  
END$$


USE `progetto`$$
DROP TRIGGER IF EXISTS `progetto`.`ConducentiNonAssociati` $$
USE `progetto`$$
CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`ConducentiNonAssociati` BEFORE INSERT ON `TurniLavorativi`  FOR EACH ROW
BEGIN
	declare var_Malato binary;
    declare var_Veicolo char(4);
    set var_Malato=b'0';
    
	select `InMalattia`, `VeicoloPubblico_Matricola`
    from `Conducente`
    where `CF`=NEW.Conducente_CF into var_Malato, var_Veicolo;
    
    if var_Malato=1 then
		signal sqlstate '45013' set message_text="Si vuole inserire l'orario di un conducente malato"; 
     end if;
     if var_Veicolo is null then
		signal sqlstate '45013' set message_text="Si vuole inserire l'orario di un conducente non associato ad alcun veicolo"; 
     end if;

		
END$$


USE `progetto`$$
DROP TRIGGER IF EXISTS `progetto`.`OrariDisgiunti` $$
USE `progetto`$$
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
        if time_to_sec(timediff(new.InizioTurno,var_fineTurno))<0  then
			if time_to_sec(timediff(new.FineTurno,var_inizioTurno))>0 then
			signal sqlstate '45014' set message_text = 'Turno di lavoro non disgiunto con quelli già presenti';
			end if;
		end if;
        if done then 
         leave read_loop;
         end if;
	end loop;
    close cur;
	
    
END$$


USE `progetto`$$
DROP TRIGGER IF EXISTS `progetto`.`max turni per conducente per l'update` $$
USE `progetto`$$
CREATE DEFINER = CURRENT_USER TRIGGER `progetto`.`max turni per conducente per l'update` BEFORE UPDATE ON `TurniLavorativi`  FOR EACH ROW
BEGIN
	declare counter int; 
    set counter=0;
    select count(*)
    from `TurniLavorativi` 
    where `Conducente_CF` = NEW.conducente_CF into counter;
    
    if counter > 4 then
     signal sqlstate '45011' set message_text = 'troppi turni per quel conducente'; 
     end if;
END$$


DELIMITER ;
-- begin attached script 'script'
delimiter !
create function `valid_conducente`(var_conducenteCF char(16),var_InizioTurno datetime, var_FineTurno datetime)
returns bool
deterministic
begin

	declare done int default false; 
    declare var_inizioTurnoCursore datetime;
	declare var_fineTurnoCursore datetime;
	declare cur cursor for select `InizioTurno`,`FineTurno`
							from `TurniLavorativi`  
                            where `Conducente_CF`=var_ConducenteCF;
	declare continue handler for not found set done= true;      
    
     open cur;
     read_loop:loop
		fetch cur into var_inizioTurnoCursore, var_fineTurnoCursore;
        if time_to_sec(timediff(var_InizioTurno,var_fineTurnoCursore))<0  then
			if time_to_sec(timediff(var_FineTurno,var_inizioTurnoCursore))>0 then
				return false;
			end if;
		end if;
        if done then 
         leave read_loop;
         end if;
	end loop;
    close cur;
return true;
end!
delimiter ;
-- end attached script 'script'
-- begin attached script 'script1'
set global event_scheduler = on;
create event if not exists `BigliettiUtilizzati`
on schedule
every 3 day
on completion preserve
comment 'Management--> Eliminazione Biglietti utilizzati'
do	
    delete from `BigliettoElettronico` where `Utilizzato`=b'1';
-- end attached script 'script1'
-- begin attached script 'script2'
set global event_scheduler = on;
delimiter !
create event if not exists `AbbonamentiUtilizzati`
on schedule
every 3 day
on completion preserve
comment 'Management--> Eliminazione Abbonamenti utilizzati'
do	
	BEGIN 
    delete from `Abbonamento` where `UltimoUtilizzo` is not null;
    END!
-- end attached script 'script2'
