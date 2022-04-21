FROM debian
MAINTAINER Leon Etienne

RUN apt-get update

RUN apt-get install -y \
  python3 \
  python3-pip

RUN pip install \
  yt-dlp

RUN apt-get install -y \
  ffmpeg

COPY ./entrypoint.sh /app/entrypoint.sh
RUN chmod +x /app/entrypoint.sh

WORKDIR /app/

ENTRYPOINT ["/app/entrypoint.sh"]
CMD ["./Tubio"]

