<?php

	//error_reporting(E_ERROR | E_PARSE);

	require_once "lib_db.php";

	$db = create_db();

	$db->exec('BEGIN');

	$res = $db->query('DELETE FROM "address_device" WHERE device="'.$_GET['device'].'" AND address="'.$_GET['address'].'"');

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