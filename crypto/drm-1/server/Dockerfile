FROM python:3.8.5-slim-buster

RUN pip install flask gunicorn pycryptodome
COPY . /app
WORKDIR /app

EXPOSE 5000

CMD ["gunicorn", "-b0.0.0.0:5000", "-w2", "-t4", "app:app"]
