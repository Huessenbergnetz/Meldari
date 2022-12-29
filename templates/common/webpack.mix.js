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

mix.copy('node_modules/es-module-shims/dist/es-module-shims.js', 'js/es-module-shims.js').minify('js/es-module-shims.js');

mix.copy('domains.js', 'js').minify('js/domains.js');
mix.copy('general.js', 'js').minify('js/general.js');
mix.copy('users.js', 'js').minify('js/users.js');
mix.copy('usersettings.js', 'js').minify('js/usersettings.js');
mix.copy('utils.js', 'js').minify('js/utils.js');