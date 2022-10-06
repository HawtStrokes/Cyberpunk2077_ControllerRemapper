#! /bin/bash

if [ ! -d "../Build/includes" ]
then
  echo "Creating Include Directory..."
  mkdir ../Build/includes
else
    echo "Directory exists"
    echo "Removing Old Files..."
    rm -rf ../Build/includes/ && mkdir ../Build/includes
fi


echo "Adding Header Files..."
cp -r ../HawtLib/Source ../Build/includes/HawtLib && find ../Build/includes/HawtLib -type f ! -iname '*.h' -delete
echo "Done!"
