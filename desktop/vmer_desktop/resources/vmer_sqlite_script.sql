
PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;

DROP TABLE IF EXISTS `element`;
CREATE TABLE `element` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `name` varchar(64) NOT NULL,
  `std_image` varchar(64) NOT NULL,
  `image` varchar(64) NOT NULL,
  `description` text
);

INSERT INTO `element` VALUES (1,'Machine Element 01','std_num_1.jpg','ele01.jpg',NULL)
  ,(2,'Machine Element 02','std_num_2.jpg','ele02.jpg',NULL)
  ,(3,'Machine Element 03','std_num_3.png','ele03.jpg',NULL)
  ,(4,'Machine Element 04','std_num_4.jpg','ele02.jpg',NULL);

-- Measuring points --
DROP TABLE IF EXISTS `point`;
CREATE TABLE `point` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `element_id` INTEGER NOT NULL,
  `name` varchar(64) NOT NULL,
  `config` text,
  `description` text
);

INSERT INTO `point` VALUES (1,1,'Point 01','{"V":1,"H":0}',NULL)
  ,(2,1,'Point 02','{"V":2,"H":0}',NULL)
  ,(3,1,'Point 03','{"V":3,"H":0}',NULL)
  ,(4,2,'Point 04','{"V":3,"H":0}',NULL)
  ,(5,2,'Point 05','{"V":3,"H":0}',NULL);

/*---- Defined by user ----*/

DROP TABLE IF EXISTS `db`;
CREATE TABLE `db` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `name` varchar(64) NOT NULL,
  `description` text
);

INSERT INTO `db` VALUES (1,'Database 01',NULL);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `company`;
CREATE TABLE `company` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `db_id` INTEGER NOT NULL,
  `name` varchar(64) NOT NULL,
  `description` text
);

CREATE INDEX `db_id` on company(`db_id`);

INSERT INTO `company` VALUES (1,1,'Company 11',NULL);
INSERT INTO `company` VALUES (2,1,'Company 12',NULL);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `plant`;
CREATE TABLE `plant` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `company_id` INTEGER NOT NULL,
  `name` varchar(64) NOT NULL,
  `description` text
);

CREATE INDEX `company_id` on plant(`company_id`);

INSERT INTO `plant` VALUES (1,1,'Plant 11',NULL);
INSERT INTO `plant` VALUES (2,1,'Plant 12',NULL);
INSERT INTO `plant` VALUES (3,2,'Plant 21',NULL);
INSERT INTO `plant` VALUES (4,2,'Plant 22',NULL);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `machine_shop`;
CREATE TABLE `machine_shop` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `plant_id` INTEGER NOT NULL,
  `name` varchar(64) NOT NULL,
  `description` text
);

CREATE INDEX `plant_id` on machine_shop(`plant_id`);

INSERT INTO `machine_shop` VALUES (1,1,'Machine Shop 11',NULL);
INSERT INTO `machine_shop` VALUES (2,1,'Machine Shop 12',NULL);
INSERT INTO `machine_shop` VALUES (3,2,'Machine Shop 21',NULL);
INSERT INTO `machine_shop` VALUES (4,3,'Machine Shop 31',NULL);
INSERT INTO `machine_shop` VALUES (5,3,'Machine Shop 32',NULL);
INSERT INTO `machine_shop` VALUES (6,4,'Machine Shop 41',NULL);
INSERT INTO `machine_shop` VALUES (7,4,'Machine Shop 42',NULL);
INSERT INTO `machine_shop` VALUES (8,4,'Machine Shop 43',NULL);

-- --------------------------------------------------------

-- Machine model --
DROP TABLE IF EXISTS `model`;
CREATE TABLE `model` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `name` varchar(64) NOT NULL,
  `description` text
);

INSERT INTO `model` VALUES (1,'Machine Model 01',NULL)
  ,(2,'Machine Model 02',NULL);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `element_in_model`;
CREATE TABLE `element_in_model` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `model_id` INTEGER NOT NULL,
  `element_id` INTEGER NOT NULL,
  `name` varchar(64) NOT NULL,
  `description` text
);

CREATE INDEX `model_id` on element_in_model(`model_id`);

INSERT INTO `element_in_model` VALUES (1,1,1,'Element 001',NULL)
  ,(2,1,2,'Element 002',NULL);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `machine`;
CREATE TABLE `machine` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `model_id` INTEGER NOT NULL,
  `name` varchar(64) NOT NULL,
  `serial_number` varchar(32) NOT NULL,
  `description` text
);

CREATE INDEX `model_id2` on machine(`model_id`);

INSERT INTO `machine` VALUES (1,1,'Machine 001','M-01',NULL);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `route`;
CREATE TABLE `route` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `plant_id` INTEGER NOT NULL,
  `name` varchar(64) NOT NULL,
  `description` text
);

CREATE INDEX `plant_id` on route(`plant_id`);

INSERT INTO `route` VALUES (1,1,'Route 01',NULL);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `machine_in_route`;
CREATE TABLE `machine_in_route` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `route_id` INTEGER NOT NULL,
  `machine_id` INTEGER NOT NULL,
  `description` text
);

CREATE INDEX `route_id` on machine_in_route(`route_id`,`machine_id`);

INSERT INTO `machine_in_route` VALUES (1,1,1,NULL);

-- --------------------------------------------------------

DROP TABLE IF EXISTS `point_in_route`;
CREATE TABLE `point_in_route` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT,
  `route_id` INTEGER NOT NULL,
  `machine_id` INTEGER NOT NULL,
  `element_id` INTEGER NOT NULL,
  `point_id` INTEGER NOT NULL,
  `config` text
);

CREATE INDEX `route_id2` on point_in_route(`route_id`,`machine_id`,`element_id`,`point_id`);

INSERT INTO `point_in_route` VALUES (1,1,1,1,1,'{"V":90,"H":0}')
  ,(2,1,1,1,2,'{"V":90,"H":0}')
  ,(3,1,1,1,3,'{"V":90,"H":0}')
  ,(4,1,1,2,4,'{"V":90,"H":0}')
  ,(5,1,1,2,5,'{"V":90,"H":0}');

COMMIT;
