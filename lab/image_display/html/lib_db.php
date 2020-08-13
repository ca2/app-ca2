<?php


	function net_prefix()
	{

		return "192.168.1.";

	}

	function create_db()
	{

		$db = new SQLite3('device.sqlite', SQLITE3_OPEN_CREATE | SQLITE3_OPEN_READWRITE);

		$db->query('CREATE TABLE IF NOT EXISTS "device" ("id" TEXT PRIMARY KEY NOT NULL, "name" TEXT)');

		$db->query('CREATE UNIQUE INDEX IF NOT EXISTS "unique_name" ON "device"("name")');

		$db->query('CREATE TABLE IF NOT EXISTS "address_device" ("address" TEXT PRIMARY KEY NOT NULL, "device" TEXT NOT NULL)');

		return $db;

	}

?>