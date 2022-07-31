<?php

//var_dump($_SERVER);
//print_r($_SERVER);
//var_dump($HTTP_RAW_POST_DATA);

var_dump($_FILES);

// foreach($_SERVER as $key => $value){
//       echo "<tr><td>" . $key . ":</td><td>" . $value . "</td><tr><br/>";
//    }

if(file_exists($target_file)) {
    chmod($target_file,0755); //Change the file permissions if allowed
    unlink($target_file); //remove the file
	echo("\n" . $target_file . "deleted.\n");
}

$target_dir = "uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
  echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
  if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
    echo "\nThe file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.";
  } else {
    echo "Sorry, there was an error uploading your file.";
  }
}
?>
