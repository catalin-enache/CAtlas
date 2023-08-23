

# https://superuser.com/questions/387619/overcoming-the-1024-character-limit-with-setx

$relativePath = "ImageMagick-7.1.1-Q16-HDRI\"
$currentDir = Get-Location
$pathToAdd = ";$currentDir\$relativePath;"

# Get the current PATH value
$existingPath = [Environment]::GetEnvironmentVariable("PATH", "User")

# Append the relative path to the existing PATH while preserving the original paths
# $newPath = "$existingPath$pathToAdd"
$newPath = $existingPath


echo "Existing PATH: "
echo $existingPath
echo " "
echo "Path to add: $pathToAdd"
echo " "


If (!$newPath.contains($pathToAdd)) {
    #$newPath += $path2add
    #$newPath = $newPath -join ';'
	$newPath = "$existingPath$pathToAdd"
    [Environment]::SetEnvironmentVariable('PATH', $newPath, 'User');
    echo "Added to PATH!"
	echo " "
} else {
	echo "Path already existed"
	echo " "
}

echo $newPath
echo " "


# Clean PATH

$existingPath = [Environment]::GetEnvironmentVariable('Path', 'User');

while ($existingPath.contains(';;')) {
    $existingPath = $existingPath.replace(';;', ';')
}

[Environment]::SetEnvironmentVariable('PATH', $existingPath, 'User');

echo "Cleaned PATH"
echo $existingPath





