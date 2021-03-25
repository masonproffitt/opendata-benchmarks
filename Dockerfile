FROM rootproject/root:6.22.08-ubuntu20.04
COPY macros /root/macros/
COPY util /root/util/
WORKDIR /root/util
ENTRYPOINT ["bash"]
