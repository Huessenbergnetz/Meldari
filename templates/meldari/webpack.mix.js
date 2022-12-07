let mix = require('laravel-mix');

mix.options({
    processCssUrls: false,
    terser: {
        terserOptions: {
            output: {
                comments: /@license/i
            }
        }
    }
});

mix.sass('assets/sass/style.scss', 'static/css/');

mix.scripts([
    'assets/js/licenseheader.js',
    'node_modules/@popperjs/core/dist/umd/popper.js',
    'node_modules/bootstrap/dist/js/bootstrap.js',
    'assets/js/general.js',
    'assets/js/validUntilInput.js',
    'assets/js/users.js',
    'assets/js/usersList.js',
    'assets/js/usersAdd.js',
    'assets/js/usersRemove.js',
    'assets/js/usersEdit.js',
    'assets/js/usersettings.js'
], 'static/js/scripts.js');

mix.copy('node_modules/bootstrap-icons/font/fonts', 'static/fonts');
