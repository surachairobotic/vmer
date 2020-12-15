
SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

-- --------------------------------------------------------


CREATE TABLE `factory` (
  `id` int(10) UNSIGNED NOT NULL,
  `name` varchar(64) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `factory` (`id`, `name`) VALUES (1,'Factory 01');
INSERT INTO `factory` (`id`, `name`) VALUES (2,'Factory 02');

-- --------------------------------------------------------


CREATE TABLE `group` (
  `id` int(10) UNSIGNED NOT NULL,
  `factory_id` int(10) UNSIGNED NOT NULL,
  `name` varchar(64) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `group` (`id`, `factory_id`, `name`) VALUES (1,1,'Group 01');
INSERT INTO `group` (`id`, `factory_id`, `name`) VALUES (2,1,'Group 02');
INSERT INTO `group` (`id`, `factory_id`, `name`) VALUES (3,2,'Group 01');

-- --------------------------------------------------------

CREATE TABLE `machine_type` (
  `id` int(10) UNSIGNED NOT NULL,
  `name` varchar(64) NOT NULL,
  `detail` TEXT NULL DEFAULT NULL,
  `image` varchar(64) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `machine_type` (`id`, `name`, `detail`, `image`) VALUES (1,'Type 01','Detail','type01.jpg');
INSERT INTO `machine_type` (`id`, `name`, `detail`, `image`) VALUES (2,'Type 02','Detail','type02.jpg');
INSERT INTO `machine_type` (`id`, `name`, `detail`, `image`) VALUES (3,'Type 03','Detail','type03.jpg');

-- --------------------------------------------------------

CREATE TABLE `machine` (
  `id` int(10) UNSIGNED NOT NULL,
  `group_id` int(10) UNSIGNED NOT NULL,
  `type_id` int(10) UNSIGNED NOT NULL,
  `name` varchar(64) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `machine` (`id`, `group_id`, `type_id`, `name`) VALUES (1,1,1,'Machine 01');
INSERT INTO `machine` (`id`, `group_id`, `type_id`, `name`) VALUES (2,2,2,'Machine 02');
INSERT INTO `machine` (`id`, `group_id`, `type_id`, `name`) VALUES (3,1,2,'Machine 03');
INSERT INTO `machine` (`id`, `group_id`, `type_id`, `name`) VALUES (4,3,3,'Machine 04');

-- --------------------------------------------------------

CREATE TABLE `route` (
  `id` int(10) UNSIGNED NOT NULL,
  `day` int(10) UNSIGNED NOT NULL,
  `factory_id` int(10) UNSIGNED NOT NULL,
  `name` varchar(64) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `route` (`id`, `day`, `factory_id`, `name`) VALUES (1,1,1,'Route 01');
INSERT INTO `route` (`id`, `day`, `factory_id`, `name`) VALUES (2,2,1,'Route 02');
INSERT INTO `route` (`id`, `day`, `factory_id`, `name`) VALUES (3,3,2,'Route 03');

-- --------------------------------------------------------

CREATE TABLE `route_machine` (
  `id` int(10) UNSIGNED NOT NULL,
  `route_id` int(10) UNSIGNED NOT NULL,
  `machine_id` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `route_machine` (`id`, `route_id`, `machine_id`) VALUES (1,1,1);
INSERT INTO `route_machine` (`id`, `route_id`, `machine_id`) VALUES (2,1,2);
INSERT INTO `route_machine` (`id`, `route_id`, `machine_id`) VALUES (3,2,1);
INSERT INTO `route_machine` (`id`, `route_id`, `machine_id`) VALUES (4,2,3);
INSERT INTO `route_machine` (`id`, `route_id`, `machine_id`) VALUES (5,3,4);

-- --------------------------------------------------------

--
-- Indexes for dumped tables
--

ALTER TABLE `factory`
  ADD PRIMARY KEY (`id`);


ALTER TABLE `group`
  ADD PRIMARY KEY (`id`),
  ADD KEY `factory_id` (`factory_id`);

ALTER TABLE `machine_type`
  ADD PRIMARY KEY (`id`);

ALTER TABLE `machine`
  ADD PRIMARY KEY (`id`),
  ADD KEY `group_id` (`group_id`);

ALTER TABLE `route`
  ADD PRIMARY KEY (`id`),
  ADD KEY `factory_id` (`factory_id`);

ALTER TABLE `route_machine`
  ADD PRIMARY KEY (`id`),
  ADD KEY `route_id` (`route_id`),
  ADD KEY `machine_id` (`machine_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `location`
--
ALTER TABLE `factory`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

ALTER TABLE `group`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

ALTER TABLE `machine_type`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

ALTER TABLE `machine`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

ALTER TABLE `route`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

ALTER TABLE `route_machine`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

