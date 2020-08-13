<?php

	error_reporting(E_ERROR | E_PARSE);

	require_once "lib_db.php";

	$db = create_db();

	$db->exec('BEGIN');

	$res = $db->query('REPLACE INTO "device" ("id", "name") VALUES (\''.$_GET['id'].'\', \''.$_GET['newname'].'\')');

	$db->exec('COMMIT');

	$db->close();

	if($res)
	{

		echo "OK";

	}
	else
	{

		echo "Fail";

	}

?>