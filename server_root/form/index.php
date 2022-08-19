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
	<h1 class="text-center p-4">Form (Query String) 📄</h1>

	<?php
    foreach ($_GET as $key => $value) {
        echo '<h1 class="text-center">' . $key . ": " . $value . '</h1>';
    }
	?>

	<div class="container py-4 px-4">
		<div class="row">
			<form class="col-md-4 mx-auto" action="index.php" method="get">
				<div class="form-group row">
					<label for="value1" class="col-form-label my-1">Value 1</label>
					<div class="col-sm-10">
						<input name="value1" type="text" class="form-control">
					</div>
					<label for="value2" class="col-form-label my-1">Value 2</label>
					<div class="col-sm-10">
						<input name="value2" type="text" class="form-control">
					</div>
					<label for="value3" class="col-form-label my-1">Value 3</label>
					<div class="col-sm-10">
						<input name="value3" type="text" class="form-control">
					</div>
					<label for="value4" class="col-form-label my-1">Value 4</label>
					<div class="col-sm-10">
						<input name="value4" type="text" class="form-control">
					</div>
				</div>
				<button type="submit" class="btn btn-block btn-primary">Send</button>
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
