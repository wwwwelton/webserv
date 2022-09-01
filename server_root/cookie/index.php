<!DOCTYPE html>
<html lang="pt-BR">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">

	<!-- Bootstrap CSS -->
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">

	<title>WebServ</title>
</head>

<body class="bg-light">
	<h1 class="text-center p-4">Cookies 🍪</h1>

	<?php
	$cookie_name = "cookie";
	if ($_SERVER["REQUEST_METHOD"] == "POST") {
		$cookie_value = $_POST["cookie_value"];
	}
	else
	{
		$cookie_value = "";
	}


	if ($cookie_value) {
		setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/");
		header("Refresh:0");
	}

	if (!isset($_COOKIE[$cookie_name])) {
		echo '<h1 class="text-center p-4">Cookie was not set.</h1>';
	} else {
		echo '<h1 class="text-center p-4">Cookie: ' . $_COOKIE[$cookie_name] . '</h1>';
	}
	?>

	<div class="container py-4 px-4">
		<div class="row">
			<form class="form-inline mx-auto" action="index.php" method="post">
				<div class="form-group mx-2">
					<label for="cookie_name" class="sr-only">Cookie Name</label>
					<input type="text" readonly class="form-control-plaintext" id="cookie_name" value="Cookie Name">
				</div>
				<div class="form-group mx-sm-3 mx-2">
					<label for="cookie_value" class="sr-only">Value</label>
					<input type="text" name="cookie_value" class="form-control" id="cookie_value" placeholder="Cookie Value">
				</div>
				<button type="submit" class="btn btn-success mx-2">Set Cookie</button>
				<a class="btn btn-danger mx-2" href="delete.php" role="button">Delete Cookie</a>
			</form>
		</div>
	</div>

	<div style="height: 200px;"></div>

	<footer class="footer fixed-bottom bg-dark p-3">
		<div class="container">
			<p class="text-white">Copyright(c) 2022 Turbo Development Design (TDD)</p>
		</div>
	</footer>

	<!-- Optional JavaScript -->
	<!-- jQuery first, then Popper.js, then Bootstrap JS -->
	<script src="https://code.jquery.com/jquery-3.2.1.slim.min.js" integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN" crossorigin="anonymous"></script>
	<script src="https://cdn.jsdelivr.net/npm/popper.js@1.12.9/dist/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script>
	<script src="https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script>
</body>

</html>
