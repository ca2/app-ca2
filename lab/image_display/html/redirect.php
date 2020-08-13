<?php

	error_reporting(E_ERROR | E_PARSE);

	require_once "lib_db.php";

   $db = create_db();
   
	$sql = 'SELECT "id", "name", "address" FROM "device" LEFT OUTER JOIN "address_device" ON "device"."id" = "address_device"."device" WHERE "address" LIKE "'.net_prefix().'%" AND name LIKE "'.$_GET['name'].'" ORDER BY "name", "address"';

   $result = $db->query($sql);

	if($result)
	{

      $row = $result->fetchArray(SQLITE3_NUM);
      
      if($row)
      {
		
         header('Location: http://'.$row[2].'/'.$_GET["script"].'?command='.$_GET['command']);

      }

   }

?>