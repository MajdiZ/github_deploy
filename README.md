# Github Deploy

## Description
Github deploy used to update your site repository from github using github [Webhooks.](https://developer.github.com/webhooks/)
Whenever you push code to your github repository, Github will make post request to Drupal site and fire updating your site repository.

## Features
- Validating payloads from GitHub.
- Whitelist requests for GitHub's IP address.
- Provide hooks during the process.
- Runs git commands using `proc_open`.
- Clear drupal cache after updating repository.
- Revert all features after successful deploy.

## Requirements
- [Libraries API](https://www.drupal.org/project/libraries)
- [Git.php](https://github.com/kbjr/Git.php) library
- [GitHub account](https://github.com/)

## Installation
1. Install [Libraries API.](https://www.drupal.org/project/libraries) **Important,** the version of the module equal to or more than **7.x-2.0**.
If you don't know how to install modules in drupal 7, please visit this [page.](https://www.drupal.org/docs/7/extending-drupal-7/installing-drupal-7-contributed-modules)
2. Enable Libraries API.
3. Install [Git.php](https://github.com/kbjr/Git.php) library. How to install the external library in drupal, you find on this [page.](https://www.drupal.org/docs/7/modules/libraries-api/installing-an-external-library-that-is-required-by-a-contributed-module)
**IMPORTANT:** the folder name of library what you installed should be called **gitphp**
4. Install `github_deploy` module and enable.
5. If you installed everything correctly, you may go into: *`admin/config/development/github-deploy`* and set up module.
If not please check installation list above.

## Configuration
1. **Origin name**: select the name of the remote repository. It look something like this (*git@github.com:your_name/your_project_name.git* or *https://github.com/your_name/your_project_name.git*).
2. **Secure method**: choose one of method to secure Github deploy callback.
3. **Clear cache**: select this checkbox if you want to clear drupal cache after deploy when callback is completed.
4. **Git bin**: in this field you need to configure the system path to your git. By default system path: *`/usr/bin/git`*.
