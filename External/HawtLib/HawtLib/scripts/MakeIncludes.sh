#! /bin/bash

if [ ! -d "../Build/Includes" ]
then
  echo "Creating Include Directory..."
  mkdir ../Build/Includes
else
    echo "Directory exists"
    echo "Removing Old Files..."
    rm -rf ../Build/Includes/ && mkdir ../Build/Includes
fi


echo "Adding Header Files..."
cp -r ../Source ../Build/Includes/HawtLib && find ../Build/Includes/HawtLib -type f ! -iname '*.h' -delete
echo "Done!"
