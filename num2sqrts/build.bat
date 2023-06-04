@echo off

xelatex num2sqrts.tex
biber num2sqrts
xelatex num2sqrts.tex
xelatex num2sqrts.tex
del *.aux *.bbl *.bcf *.blg *.log *.out *.run.xml
