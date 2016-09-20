# docker-inspircd

## Configuration Variables
`rules` - The rules for the server
`motd` - The MOTD for the server

## Configuration Example
```
InspIRCd-Server:
  ports:
  - 6667/tcp
  - 6668/tcp
  hostname: irc.shadowacre.ltd
  labels:
    io.rancher.container.pull_image: always
  tty: true
  image: r4wizard/docker-inspircd:latest
  restart: always
  environment:
    tiller_json: |
		WE SHOULD PUT THE config.json HERE
```
or
```
docker run -e environment=production -e tiller_json="$(cat config.json)" ...
```
