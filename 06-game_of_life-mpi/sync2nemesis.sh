#!/usr/bin/bash
echo 'Sincronizando com servidor'
rsync -av --progress --delete * mzamith@nemesis:/home/mzamith/Tests/MPI/01-game_of_life-mpi/
rsync -av --progress --delete * mzamith@knightmare:/home/mzamith/Tests/MPI/01-game_of_life-mpi/
