<!DOCTYPE html>
<html lang="pt-BR">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">

	<!-- Bootstrap CSS -->
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css" integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2" crossorigin="anonymous">

	<title>WebServ</title>

	<script>
		function upload() {
			var elem = document.getElementById("spinner");
			elem.classList.remove("d-none");
			elem.classList.add("d-flex");
			var elem = document.getElementById("pre_upload");
			elem.classList.add("d-none");
			var elem = document.getElementById("while_upload");
			elem.classList.remove("d-none");
			var elem = document.getElementById("send_form");
			elem.classList.add("d-none");
		}
	</script>
</head>

<body class="bg-light">
	<h1 class="text-center p-4">File Upload 🆙</h1>

	<?php
	if ($_SERVER["REQUEST_METHOD"] == "POST") {
		$target_dir = "uploads/";
		$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);

		if (file_exists($target_file)) {
			chmod($target_file, 0755);
			unlink($target_file);
		}

		if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
			echo '<h1 id="pre_upload" class="text-center p-4"><a href="' . "/uploads" . '" target="_blank" role="button">' . basename($_FILES["fileToUpload"]["name"]) . '</a></h1>';
		} else {
			echo '<h1 id="pre_upload" class="text-center text-danger p-4">An error occurred when moving the file!</h1>';
		}
	} else {
		echo '<h1 id="pre_upload" class="text-center p-4">Your file will appear here.</h1>';
	}

	echo '<h1 id="while_upload" class="d-none text-center p-4">Uploading...</h1>';
	echo '<div id="spinner" class="d-none justify-content-center">
				<div class="spinner-border"></div>
			</div>';

	?>

	<div id="send_form" class="container py-4 px-4">
		<div class="row">
			<form class="col-md-4 mx-auto" action="index.php" method="post" enctype="multipart/form-data">
				<div class="form-group">
					<label for="exampleFormControlFile1">Select file to upload:</label>
					<input type="file" name="fileToUpload" class="form-control-file" id="exampleFormControlFile1">
				</div>
				<button onclick="upload()" type="submit" class="btn btn-block btn-primary">Send</button>
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
