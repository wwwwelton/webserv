<?php
function file_upload_handler() {
  header('Content-Type: text/html');
  header_remove('Content-Type');

  // foreach($_SERVER as $key => $value){
  //       echo "<tr><td>" . $key . ":</td><td>" . $value . "</td><tr><br/>";
  //    }

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
      echo "<p>The file has been uploaded, but we couldn't seve it</p>";
    }
    echo "<p>Redirecting to homepage in 10 seconds</p>";
  }
}
?>

<?php
 echo "DUMPING ALL HEADERS !!! \n";
 echo "DUMPING ALL HEADERS !!! \n";
 foreach (getallheaders() as $name => $value) {     echo "$name: $value\n"; }
 ?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8"/>
    <title>File upload</title>

    <meta http-equiv="refresh" content="10; URL=http://127.0.0.1:3490/sito/"/>
</head>
<body>
    <p>
      <?php
        file_upload_handler();
      ?>
    </p>
</body>
</html>


