<?php
    echo "\nSERVER\n";
    var_dump($_SERVER);
    echo "\nFILES\n";
    var_dump($_FILES);
    echo "\nPOST\n";
    var_dump($_POST);
    echo "\nENV\n";
    var_dump($_ENV);
    echo "\nRAW\n";
    var_dump($HTTP_RAW_POST_DATA);
    echo "\nSESSION\n";
    var_dump($_SESSION);

    echo $_ENV["CONTENT_LENGTH"];
    echo $_SERVER["CONTENT_LENGTH"];
    echo getenv("CONTENT_LENGTH");
    // echo $_FILES[0][0];
    // echo htmlspecialchars($_POST["e-mail"]);
?>
