## IMPORTANT:

You must have env variable ``JAVA_HOME`` set for the build process and program to work.

---

Use: ``javap -s -p -classpath jars/nrssl.jar ro.upb.nrs.sl.Posit_B`` to find signatures and method names.

---

Use: ``make run-test`` to run the pass on ``sample.c``

## Dockerfile

You can build the image using:

``docker build -t llvm-pass .``

Then you can start a shell using:

``docker container run -it llvm-pass bash``
