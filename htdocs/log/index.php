<?php
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["files"])) {
    $uploadDir = "/Users/rouali/Desktop/web_s/htdocs/uploads/";

    // Check if files are uploaded and if it's an array
    if (is_array($_FILES["files"]["name"])) {
        $uploadedFiles = $_FILES["files"];

        // Loop through each file
        for ($i = 0; $i < count($uploadedFiles['name']); $i++) {
            $fileName = basename($uploadedFiles["name"][$i]);
            $uploadFile = $uploadDir . $fileName;

            // Check if file already exists
            if (file_exists($uploadFile)) {
                echo "<p>File $fileName already exists.</p>";
            } else {
                // Attempt to move uploaded file
                if (move_uploaded_file($uploadedFiles["tmp_name"][$i], $uploadFile)) {
                    echo "<p>File $fileName uploaded successfully.</p>";
                } else {
                    echo "<p>Failed to upload file $fileName.</p>";
                }
            }
        }
    } else {
        echo "<p>No files uploaded or invalid file input.</p>";
    }
} else {
    echo "<p>No POST request or files uploaded.</p>";
}
?>