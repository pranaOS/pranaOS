local helpers = require('helpers')
local paths = require('paths')

local factory = {}

factory.script_name = "factory.lua"

local function remove_old_database() 
    print(string.format("Removing old databases from %s", paths.db_dir))
    helpers.rm_files_from_dir(paths.db_dir)
end

local function copy_clean_database()
end

local function remove_cache() 
    if helpers.exists(paths.file_indexer_cache) then 
        print(string.format("Removing cache file: %s", paths.file_indexer_cache))
        assert(os.remove(paths.file_indexer_cache))
    end
end

function factory.execute()
    remove_old_database()
    copy_clean_database()
    remove_cache()
end