<?php
if ($_SERVER["REQUEST_METHOD"] == "GET") {
	foreach ($_GET as $key => $value) {
		echo $key . "=" . $value . "-";
	}
}
if ($_SERVER["REQUEST_METHOD"] == "POST") {
	parse_str($_SERVER["QUERY_STRING"], $query_string);
	foreach ($query_string as $key => $value) {
		echo $key . "=" . $value . "-";
	}
}

?>
