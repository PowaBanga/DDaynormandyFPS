--
-- Q2Admin example configuration
-- 
-- rename to "config.lua" and place this to quake 2 root
--

plugins = {
    lrcon = {
        quit_on_empty = true,
        cvars = {
            -- server
            'password', 'maxclients', 'timelimit', 'dmflags', 'sv_gravity', 'sv_iplimit', 'fraglimit',
            'sv_anticheat_required','sv_fps',

            -- mod
            'teamplay', 'ctf', 'matchmode', 'roundtimelimit', 'tgren', 'limchasecam', 'forcedteamtalk',
            'mm_forceteamtalk', 'ir', 'wp_flags', 'itm_flags', 'hc_single', 'use_punch',  'darkmatch',
            'allitem', 'allweapon', 'use_3teams'
        }
    },
    coinflip = {} -- Heads & Tails script
}
