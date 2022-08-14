<?php
  $target_dir = "uploads/";
  mkdir($target_dir, 0755);

  $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);

  if(file_exists($target_file)) {
      chmod($target_file, 0755);
      unlink($target_file);
      echo("<p>" . $target_file .
           "previous file with the same name deleted.</p>");
  }

  $uploadOk = 1;
  $imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

  // Check if $uploadOk is set to 0 by an error
  if ($uploadOk == 0) {
    echo "<p> Sorry, your file was not uploaded. </p>";
  // if everything is ok, try to upload file
  } else {
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
      echo "<p>" . htmlspecialchars(basename($_FILES["fileToUpload"]["name"])).
           " has been uploaded. </p>";
    } else {
      echo "<p>Sorry, there was an error uploading your file.</p>";
    }
    echo "<p>Redirecting to homepage in 10 seconds</p>";
  }
?>
