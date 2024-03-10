function navigate_and_rsync {
    local dir="$1"
    cd "$dir" || exit

    # Loop through all subdirectories
    for subdir in */; do
        if [ -d "$subdir" ]; then
            echo "Entering $subdir"
            cd "$subdir" || exit
            # Execute rsync command
            rsync -av --ignore-existing ./* /usr/local/
            cd ..
            echo "Exited $subdir"
            rm -rd "$subdir"
        fi
    done
}

# Start navigating from the specified path
starting_path="/dependencies"

if [ -d "$starting_path" ]; then
    echo "Starting navigation from: $starting_path"
    navigate_and_rsync "$starting_path"
else
    echo "Invalid starting path: $starting_path"
fi