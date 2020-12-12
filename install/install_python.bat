:: Script for installing opencv prerequisites in an Anaconda virtual env

:: create virtual environment
call conda remove -y -n opencv --all
call conda create -y -n opencv python
call activate opencv
 
:: install packages
call conda install numpy -y
call conda install pandas -y
call conda install pyodbc -y
call conda install -c conda-forge pyfiglet -y
call conda install pyyaml -y
call pip install opencv-python

:: pause
call pause