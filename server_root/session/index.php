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
	<h1 class="text-center p-4">Session 📃</h1>

	<?php
	session_start();

	if ($_SERVER["REQUEST_METHOD"] == "POST") {
		$username = $_POST["username"];
	}
	else
	{
		$username = "";
	}

	if ($username) {
		$_SESSION["username"] = $username;
	}

	if (!isset($_SESSION["username"])) {
		echo '<h1 class="text-center p-4">You are not logged in.</h1>';
	} else {
		echo '<h1 class="text-center p-4">' . $_SESSION["username"] . ' is logged in. </h1>';
	}
	?>

	<div class="container py-4 px-4 <?php if (isset($_SESSION["username"])) {
										echo "d-none";
									} ?>">
		<div class="row">
			<form class="col-md-3 mx-auto" action="index.php" method="post">
				<div class="form-group">
					<label for="username">Username</label>
					<input type="text" class="form-control" name="username" id="username" placeholder="Username" required>
				</div>
				<div class="form-group">
					<label for="password">Password</label>
					<input type="password" class="form-control" name="password" id="password" placeholder="Password" required>
				</div>
				<button type="submit" class="btn btn-block btn-primary">Login</button>
			</form>
		</div>
	</div>

	<div class="container py-4 px-4 <?php if (!isset($_SESSION["username"])) {
										echo "d-none";
									} ?>">
		<div class="row">
			<div class="col-md-3 p-1 mx-auto">
				<img src="https://w7.pngwing.com/pngs/442/715/png-transparent-check-mark-computer-icons-icon-design-cheque-successful-angle-logo-grass.png" class="img-fluid img-thumbnail w-100 mb-4" alt="Responsive image">
				<a class="btn btn-block btn-danger" href="delete.php" role="button">Logout</a>
			</div>
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
