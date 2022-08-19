<?php
  $target_dir = "uploads/";

  $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);

  if(file_exists($target_file)) {
      chmod($target_file, 0755);
      unlink($target_file);
      echo(basename($_FILES["fileToUpload"]["name"]) .  " DELETED</br>");
  }

  if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
	echo htmlspecialchars(basename($_FILES["fileToUpload"]["name"])).
		 " OK";
  } else {
	echo htmlspecialchars(basename($_FILES["fileToUpload"]["name"])).
		 "ERROR";
  }
