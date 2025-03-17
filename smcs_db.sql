-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               8.0.39 - MySQL Community Server - GPL
-- Server OS:                    Win64
-- HeidiSQL Version:             12.0.0.6468
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


-- Dumping database structure for smcs_db
CREATE DATABASE IF NOT EXISTS `smcs_db` /*!40100 DEFAULT CHARACTER SET utf8mb3 */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `smcs_db`;

-- Dumping structure for table smcs_db.auto_dispense
CREATE TABLE IF NOT EXISTS `auto_dispense` (
  `id` int NOT NULL AUTO_INCREMENT,
  `status` varchar(5) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb3;

-- Dumping data for table smcs_db.auto_dispense: ~0 rows (approximately)
INSERT INTO `auto_dispense` (`id`, `status`) VALUES
	(1, 'off');

-- Dumping structure for table smcs_db.dispense_mode
CREATE TABLE IF NOT EXISTS `dispense_mode` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb3;

-- Dumping data for table smcs_db.dispense_mode: ~2 rows (approximately)
INSERT INTO `dispense_mode` (`id`, `name`) VALUES
	(1, 'auto'),
	(2, 'manual');

-- Dumping structure for table smcs_db.sensor_data
CREATE TABLE IF NOT EXISTS `sensor_data` (
  `id` int NOT NULL AUTO_INCREMENT,
  `supplied_time` datetime NOT NULL,
  `sensor_reading` int NOT NULL,
  `plot` varchar(5) NOT NULL,
  `dispense_mode_id` int NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_sensor_data_dispense_mode_idx` (`dispense_mode_id`),
  CONSTRAINT `fk_sensor_data_dispense_mode` FOREIGN KEY (`dispense_mode_id`) REFERENCES `dispense_mode` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=122 DEFAULT CHARSET=utf8mb3;

-- Dumping data for table smcs_db.sensor_data: ~14 rows (approximately)
INSERT INTO `sensor_data` (`id`, `supplied_time`, `sensor_reading`, `plot`, `dispense_mode_id`) VALUES
	(106, '2024-12-20 17:12:14', 765, 'B', 1),
	(107, '2024-12-20 17:15:58', 765, 'A', 2),
	(108, '2024-12-20 17:16:06', 765, 'A', 2),
	(109, '2024-12-20 17:16:14', 765, 'A', 2),
	(110, '2024-12-20 17:17:25', 765, 'B', 2),
	(111, '2024-12-20 17:17:28', 765, 'A', 2),
	(112, '2024-12-21 10:56:55', 765, 'A', 2),
	(113, '2024-12-21 10:57:13', 765, 'B', 1),
	(114, '2024-12-21 10:57:32', 320, 'B', 2),
	(115, '2024-12-21 10:57:57', 765, 'A', 1),
	(116, '2024-12-21 10:58:17', 765, 'A', 1),
	(117, '2024-12-21 12:42:05', 765, 'B', 2),
	(118, '2024-12-21 12:42:40', 765, 'A', 1),
	(119, '2024-12-21 12:43:59', 765, 'A', 1),
	(120, '2024-12-21 12:44:27', 157, 'B', 2),
	(121, '2024-12-21 12:44:29', 116, 'A', 2);

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
