local helpers = require('helpers')
local paths = require('paths')

local factory = {}
factory.script_name = "factory.lua"

-- removeOldDatabase
local function remove_old_databases()
    print(string.format("Removing old databases from '%s'", paths.db_dir))
    helpers.rm_files_from_dir(paths.db_dir)
end

-- copyCleanDatabase
local function copy_clean_databases()
    print(string.format("Copying clean databases from '%s' to '%s'", paths.db_factory_dir, paths.db_dir))
    helpers.copy_dir(paths.db_factory_dir, paths.db_dir)
end

-- removeCache
local function remove_cache()
    if helpers.exists(paths.file_indexer_cache) then
        print(string.format("Removing cache file '%s'", paths.file_indexer_cache))
        assert(os.remove(paths.file_indexer_cache))
    end
end

-- functions which executes
function factory.execute()
    remove_old_databases()
    copy_clean_databases()
    remove_cache()
end

return factory