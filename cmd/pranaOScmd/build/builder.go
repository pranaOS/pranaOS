package build

type Config struct {
	WorkDir        string
	GoRoot         string
	Basedir        string
	BuildTest      bool
	pranaOSVersion string
	GoArgs         []string
}

type Builder struct {
	cfg     Config
	basedir string
}
