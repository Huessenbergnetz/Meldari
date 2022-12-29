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

mix.copy('node_modules/bootstrap/dist/js/bootstrap.bundle.*', 'static/js');

mix.copy('assets/js/domains.js', 'static/js').minify('static/js/domains.js');
mix.copy('assets/js/index.js', 'static/js').minify('static/js/index.js');
mix.copy('assets/js/users.js', 'static/js').minify('static/js/users.js');
mix.copy('assets/js/usersettings.js', 'static/js').minify('static/js/usersettings.js');
mix.copy('assets/js/utils.js', 'static/js').minify('static/js/utils.js');
mix.copy('assets/js/validuntilinputs.js', 'static/js').minify('static/js/validuntilinputs.js');

mix.copy('node_modules/bootstrap-icons/font/fonts', 'static/fonts');
