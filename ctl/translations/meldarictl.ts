<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" sourcelanguage="en">
<context>
    <name></name>
    <message id="melctl-err-no-command">
        <source>No command given. Use -h/--help to show available options and usage information.</source>
        <extracomment>Hint given if meldarictl has been called without any option</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-msg-done">
        <source>Done</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-msg-failed">
        <source>Failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-read-bool-short-yes">
        <source>Y</source>
        <extracomment>short for yes, answer for a boolean question</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-read-bool-short-no">
        <source>N</source>
        <extracomment>short for no, answer for a boolean question</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-read-bool-long-yes">
        <source>Yes</source>
        <extracomment>answer for a boolean question</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-read-bool-long-no">
        <source>No</source>
        <extracomment>answer for a boolean question</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-error-config-file-not-readable">
        <source>Can not read configuration file at %1</source>
        <extracomment>Error message, %1 will be replaced by the file path</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-error-config-file-malformed">
        <source>Failed to parse configuration file at %1</source>
        <extracomment>Error message, %1 will be replaced by the file path</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-status-open-db">
        <source>Establishing database connection</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-error-db-invalid-object">
        <source>Failed to obtain database object.</source>
        <extracomment>Error message</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-error-config-invalid-dbtype">
        <source>Can not open database connection for not supported database type “%1”.</source>
        <extracomment>Error message, %1 will be the invalid DB type</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-invalid-command">
        <source>&quot;%1&quot; is not a valid command. Use -h/--help to show a list of available commands.</source>
        <extracomment>CLI error</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-global-help-desc">
        <source>Displays help on commandline options and commands.</source>
        <extracomment>CLI option description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-global-inifile-desc">
        <source>Path to configuration ini file. Default: %1</source>
        <extracomment>CLI option description, %1 will be the default value</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-global-inifile-val">
        <source>ini file</source>
        <extracomment>CLI option value name</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-global-quiet-desc">
        <source>Be quiet and print less output.</source>
        <extracomment>CLI option description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-help-usage">
        <source>Usage:</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-help-usage-global-options">
        <source>[global options]</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-help-usage-command">
        <source>&lt;command&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-help-description">
        <source>Meldari is a security reporting endpoint based on Cutelyst.
This command line client helps to manage several aspects of Meldari from the command line.</source>
        <oldsource>Meldari is a security reporting endpoint based on Cutelyst. This command line client helps to manage several aspects of Meldari from the command line.</oldsource>
        <extracomment>General description for gikwimictl</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-help-header-global-options">
        <source>Global options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-help-header-commands">
        <source>Commands:</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-help-usage-options">
        <source>[options]</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-help-header-options">
        <source>Options:</source>
        <extracomment>CLI help header</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-summary">
        <source>Manage database migrations</source>
        <extracomment>CLI command summary</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-description">
        <source>Run, roll back, reset and refresh database migrations. Be careful, these commands can easily lead to data loss! Make sure that you have made a backup of your database beforehand.</source>
        <extracomment>CLI command description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-migration-summary">
        <source>Run database migrations</source>
        <extracomment>CLI command summary</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-migration-description">
        <source>Performs all database migrations that have not already be performed before. You should run this command after upgrading Meldari.</source>
        <extracomment>CLI command description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-status-reading-config">
        <source>Reading configuration file</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-database-rollback-steps-desc">
        <source>Number of migration steps to roll back. Default: %1.</source>
        <extracomment>CLI option description, %1 will be replaced by the default value</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-database-rollback-steps-value">
        <source>steps</source>
        <extracomment>CLI option value name</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-error-invalid-rollback-steps-integer">
        <source>Can not perform database migration rollback. “%1“ is not a valid integer value for steps.</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-rollback-summary">
        <source>Roll back database migrations</source>
        <extracomment>CLI command summary</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-rollback-description">
        <source>Rolls back the specified number of migration steps. Note that this will result in data loss. This command is for development purposes only.</source>
        <extracomment>CLI command description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-database-refresh-steps-desc">
        <source>Number of migration steps to refresh. If 0, all migrations will be refreshed. Default: %1.</source>
        <extracomment>CLI option description, %1 will be replaced by the default value</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-database-refresh-steps-value">
        <source>steps</source>
        <extracomment>CLI option value name</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-error-invalid-refresh-steps-integer">
        <source>Can not perform database migration refresh. “%1“ is not a valid integer value for steps.</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-refresh-summary">
        <source>Refresh database migrations</source>
        <extracomment>CLI command summary</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-refresh-description">
        <source>Refreshes the specified number of migration steps. A refresh will roll back the specified number of migrations and will rerun them afterwards. Note that this will result in data loss. This command is for development purposes only.</source>
        <extracomment>CLI command description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-reset-summary">
        <source>Reset database migrations</source>
        <extracomment>CLI command summary</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-database-reset-description">
        <source>Resets all database migrations. A reset will roll back and reapply all migrations. Note that this will result in data loss. This command is for development purposes only.</source>
        <extracomment>CLI command description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-user-summary">
        <source>Manage users</source>
        <extracomment>CLI command summary</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-user-description">
        <source>Add, edit, delete and list users.</source>
        <extracomment>CLI command description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-user-add-username-desc">
        <source>The user name for the user to create.</source>
        <extracomment>CLI option description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-user-add-username-value">
        <source>name</source>
        <extracomment>CLI option value name</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-user-add-email-desc">
        <source>The email address of the user to create.</source>
        <extracomment>CLI option description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-user-add-email-value">
        <source>email</source>
        <extracomment>CLI option value name</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-user-add-type-desc">
        <source>The type of the new user. Available types: %1. Default value: %2.</source>
        <extracomment>CLI option description, %1 will be replaced by a list of supported types, %2 by the default value</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-user-add-type-value">
        <source>type</source>
        <extracomment>CLI option value name</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-user-add-password-desc">
        <source>The password for the new user. If empty, a random password will be generated that will be displayed after the user has been created successfully.</source>
        <extracomment>CLI option description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-opt-user-add-password-value">
        <source>password</source>
        <extracomment>CLI option value name</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-status-add-user">
        <source>Adding new user</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-user-add-err-empty-name">
        <source>Can not add a user without a valid username</source>
        <extracomment>CLI error message</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-user-add-err-invalid-email">
        <source>Can not add a user without a valid email address</source>
        <extracomment>CLI error message</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-user-add-err-failed-pwenc">
        <source>Failed to encrypt password</source>
        <extracomment>CLI error message</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-user-add-err-username-in-use">
        <source>The username “%1“ is already in use</source>
        <extracomment>CLI error message, %1 will be replaced by the username</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-user-add-err-email-in-use">
        <source>The email address “%1” is already in use by “%2“</source>
        <extracomment>CLI error message, %1 will be replaced by the email address, %2 by the username</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-cli-status-generated-password">
        <source>Generated password for user “%1“: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-user-add-summary">
        <source>Add a new user</source>
        <extracomment>CLI command summary</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-command-user-add-description">
        <source>Add a new user with the given option parameters.</source>
        <extracomment>CLI command description</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message id="melctl-warn-db-set-con-tz">
        <source>Failed to set database connection time zone to UTC: %1</source>
        <oldsource>Failed to set database connection time zone to UTC: %1&apos;
</oldsource>
        <extracomment>CLI warning message</extracomment>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
