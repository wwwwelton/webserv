<?php
function file_deletion_handler() {
  header('Content-Type: text/html');
  header_remove('Content-Type');

  $target_dir = "uploads/";
  mkdir($target_dir, 0755);

  $target_file = $target_dir . basename($_FILES["fileToDelete"]["name"]);

  if(!file_exists($target_file)) {
    echo("<p>File does not exist in the database</p>");
  }
  else if (!is_writable($target_file))
    echo("<p>Insufficient permissions to delete file</p>");
  else {
    unlink($target_file);
    echo("<p>" . $target_file . " deleted succesfully</p>");
  }
}
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8"/>
    <title>File deletion</title>

    <meta http-equiv="refresh" content="10; URL=http://127.0.0.1:3490/sito/"/>
</head>
<body>
    <p>
      <?php
        file_deletion_handler();
      ?>
    </p>
</body>
</html>
