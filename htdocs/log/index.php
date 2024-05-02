<?php
if ($_SERVER["REQUEST_METHOD"] == "POST")
{
    $uploadDir = "/Users/rouali/Desktop/web_s/htdocs/uploads/";

    if (isset($_FILES["file"]) && $_FILES["file"]["error"] == 0)
    {
      $uploadFile = $uploadDir . basename($_FILES["file"]["name"]);
      echo "UPLOAD_FILE: $uploadFile"; 
      if (file_exists($uploadFile)) {
        echo "File already exists.";
      } else {
        if (move_uploaded_file($_FILES["file"]["tmp_name"], $uploadFile))
        {
          echo "File is valid, and was successfully uploaded.";
        } else
        {
          echo "Upload failed.";
        }
      }
    } else
    {
      echo "Invalid file upload.";
    }
}
?>