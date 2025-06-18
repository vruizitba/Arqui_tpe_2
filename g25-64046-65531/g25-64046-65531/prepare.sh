docker pull agodio/itba-so:2.0
docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -it --name tpe_arqui_g25_1q2025 agodio/itba-so:2.0
docker start tpe_arqui_g25_1q2025
docker exec -it tpe_arqui_g25_1q2025 make clean -C/root/Toolchain
docker exec -it tpe_arqui_g25_1q2025 make clean -C/root
docker exec -it tpe_arqui_g25_1q2025 make all -C/root/Toolchain
docker exec -it tpe_arqui_g25_1q2025 make all -C/root
docker stop tpe_arqui_g25_1q2025
sudo chown "$USER:$USER" Image/x64BareBonesImage.qcow2 2>/dev/null
sudo chmod u+rw Image/x64BareBonesImage.qcow2 2>/dev/null