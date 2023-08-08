<?php

$hostname = "localhost";
$username = "root";
$password = "";
$database = "esp32";

$conn = mysqli_connect($hostname, $username, $password, $database);

if(!$conn) {
	die("Connection failed: " . mysqli_connect_error());
}

echo"Database connection is OK<br>";	

if(isset($_POST["CO2"])) {

	$c = $_POST["CO2"];
	

	$sql = "INSERT INTO co2 (CO2) VALUES (".$c.")";

	if (mysqli_query($conn, $sql)) { 
		echo "\nNew record created successfully";
	}else{
		echo "Error: " . $sql . "<br>" . mysqli_error($conn);
	}
}
?>