# SDFiles Updater

A homebrew application for the Nintendo Switch that will connect to a SDFiles Updater Server and update your SDFiles automatically.

## settings.cfg

| Config option                                                              | Description
| -------------------------------------------------------------------------- | ---
| `host = "http://sdfu.stevenmattera.com";`                                  | Which server to connect to. You can host your own server using the files in the [server repository](https://github.com/StevenMattera/SDFilesUpdaterServer).
| `channel = "stable";`                                                      | Stable (*stable*) will download the latest tag and bleeding edge (*bleeding-edge*) will download the latest commit.
| `bundle = "sdfiles";`                                                      | The bundle to install. (`sdfiles`, `hekate`, `atmosphere`, or `reinx`)
| `version = "";`                                                            | The version of SDFiles that is currently installed on your SD card.
| `ignore = [ "sdmc:/fileToIgnore.nro", "sdmc:/anotherFileToIgnore.nro" ];`  | Array of files to ignore when extracting.
| `autoupdate = true;`                                                       | Whether or not to auto update SDFiles Updater.
| `proxy_enabled = false;`                                                   | Whether or not to use a proxy for network calls.
| `proxy_url = "http://example.com:8080";`                                   | The URL of the proxy server.
| `proxy_username = "username";`                                             | The username to use for the proxy server, if blank it will not be used.
| `proxy_password = "password";`                                             | The password to use for the proxy server, if blank it will not be used.

## Credits

* Thanks to tumGER for maintaing the SDFilesSwitch repo, without that repository this application wouldn't exist.
* Thanks to vgmoose for examples on using minizip in appstorenx.
* Thanks to y4my4m and natinusala in the ReSwitched discord for their discussions around libcurl.
