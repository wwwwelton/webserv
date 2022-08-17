<?php

session_start();
session_unset();
session_destroy();
header( "refresh:0; url=index.php" );

?>
