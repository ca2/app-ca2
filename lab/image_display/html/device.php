<html>
<head>
<title>Devices</title>
<script
  src="https://code.jquery.com/jquery-3.3.1.min.js"
  integrity="sha256-FgpCb/KJQlLNfOu91ta32o/NMZxltwRo8QtmkMRdAu8="
  crossorigin="anonymous"></script>
</head>
<h3>Devices</h3>
<body style="font-family:Geneva,Helvetica,sans-serif,sans;">
<table cellpadding="2">
	<tr style="background-color:rgb(80,80,80); color:rgb(255,255, 255);">
	<td>
		</td>
		<td>
		Name
		</td>
		<td>
		Address
		</td>
	</tr>
<?php

	error_reporting(E_ERROR | E_PARSE);

	require_once "lib_db.php";

	$db = create_db();
	
	$sql = 'SELECT "id", "name", "address" FROM "device" LEFT OUTER JOIN "address_device" ON "device"."id" = "address_device"."device" WHERE "address" LIKE "'.net_prefix().'%" ORDER BY "name", "address"';

	$result = $db->query($sql);

	$i = 0;

	if($result)
	{

		while($row = $result->fetchArray(SQLITE3_NUM))
		{

			

			if($i % 2 == 1)
			{

				$bkcolor = "rgb(235,235,235)";

			}
			else
			{

				$bkcolor = "rgb(255,255,255)";

			}

		?>
		<tr style="background-color:<? echo $bkcolor; ?>;" >
			<td>
				<section device="<? echo $row[0]; ?>" address="<? echo $row[2]; ?>">
					<input type="button" content_delete="true" value="Delete"/>
				</section>
			</td>
			<td>
				<section id="<? echo $row[0]; ?>">
					<input type="text" nameeditable="true" style="opacity:0.5;" value="<? echo $row[1]; ?>"/>
				</section>
			</td>
			<td>
				<?php

					echo $row[2];

				?>
			</td>
		</tr>

	<?php

	$i++;

	}

}
else
{

	?>

<h3 style="color:red">Error fetching devices.</h3>

	<?php

}

	$db->close();

?>
</table>
<script>

$(document).ready(function()
{    

	$('input[content_delete=true]').bind('click', function(e)
	{  

		$.ajax(
		{

			type:'GET',

			url: 'delete_device_address.php',

			data:
			{

				device: $(this).parent().attr('device') ,

				address: $(this).parent().attr('address')

			},

			success: function(msg)
			{

				if(!msg)
				{

					alert('update failure');

				}

				location.reload();

			}

		});

	});
	$('input[contenteditable=true]').bind('blur keyup', function(e)
	{  

		if (e.type === 'blur' || e.keyCode === 13)  
		{

			$.ajax(
			{

				type:'GET',

				url: 'https://' + $(this).parent().attr('address') + '/',

				data:
				{

					command: $(this).val(),

				},

				success: function(msg)
				{

					if(!msg)
					{

						alert('update failure');

					}

				}

			});
		
		}

	});
	
	$('input[fullscreen=true]').bind('blur keyup', function(e)
	{  

		if (e.type === 'blur' || e.keyCode === 13)  
		{

			$.ajax(
			{

				type:'GET',

				url: 'https://' + $(this).parent().attr('address') + '/fullscreen',

				data:
				{

					command: $(this).val(),

				},

				success: function(msg)
				{

					if(!msg)
					{

						alert('update failure');

					}

				}

			});
		
		}

	});
	$('input[nameeditable=true]').bind('blur keyup', function(e)
	{  

		if (e.type === 'blur' || e.keyCode === 13)  
		{

			$.ajax(
			{
	
				type:'GET',
	
				url: 'change_name.php',
	
				data:
				{
				
					newname: $(this).val(),
				
					id: $(this).parent().attr('id') 
				
				},
				
				success: function(msg)
				{
				
					if(!msg)
					{

						alert('update failure');

					}

					location.reload();

				}

			});
					
		}

	});

});

</script>
</body>
</html>	
