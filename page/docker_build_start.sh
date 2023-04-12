if [ "$1" = "debug" ]; then
    sudo docker build -t nginx-serve . && echo -e "\nDEBUG: STARTING SERVER CONTAINER\n"
    sudo docker run -p 80:80 nginx-serve
else
    sudo docker rm -vf $(sudo docker ps -a -q) &> /dev/null && echo -e -n "\nREMOVING DOCKER CONTAINERS" && echo "...OK"
    echo -e -n "\nREMOVING DOCKER IMAGES" && sudo docker rmi -f $(sudo docker images -aq) &> /dev/null && echo "...OK"
    echo -e "\nDOCKER PRUNE ALL" && docker system prune -a --volumes
    echo -e "\nBUILDING DOCKER IMAGE\n"
    sudo docker build -t nginx-serve . && echo -e "\nSTARTING SERVER CONTAINER\n"
    sudo docker run -p 80:80 nginx-serve
fi
