@echo off

xelatex num2sqrts.tex
xelatex num2sqrts.tex
del *.aux *.log *.out
