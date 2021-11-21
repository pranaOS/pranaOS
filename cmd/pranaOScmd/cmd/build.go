package cmd

import (
	"log"

	"github.com/pranaOS/pranaOS/cmd/egg/build"
	"github.com/spf13/cobra"
)

var (
	output string
)

var buildCmd = &cobra.Command{
	Use:                "build",
	Short:              "go build like command",
	DisableFlagParsing: true,
	Run: func(cmd *cobra.Command, args []string) {
		err := runBuild(args)
		if err != nil {
			log.Fatal(err)
		}
	},
}

func runBuild(args []string) error {
	b := build.NewBuilder(build.Config{
		GoRoot:       goroot,
		EggosVersion: pranaOSVersion,
		GoArgs:       args,
	})
	err := b.Build()
	if err != nil {
		return err
	}
	return nil
}

func init() {
	rootCmd.AddCommand(buildCmd)
}
