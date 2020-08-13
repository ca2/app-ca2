<?php

	error_reporting(E_ERROR | E_PARSE);

	require_once "lib_db.php";

	$db = create_db();

	$db->exec('BEGIN');

	$res = $db->query('INSERT INTO "device" ("id", "name") VALUES (\''.$_GET['id'].'\', \''.$_GET['address'].'\')');

	$db->exec('COMMIT');

	$db->exec('BEGIN');

	$res = $db->query('REPLACE INTO "address_device" ("address", "device") VALUES (\''.$_GET['address'].'\', \''.$_GET['id'].'\')');

	$db->exec('COMMIT');

	$db->close();

	//echo "id=".$_GET['id'];

	//echo "address=".$_GET['address'];

	if($res)
	{

		echo "OK";

	}
	else
	{

		echo "Fail";

	}

?>