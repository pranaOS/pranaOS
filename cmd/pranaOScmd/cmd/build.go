package cmd

import (
	"log"

	"github.com/spf13/cobra"
)

var (
	output string
)

var buildCmd = &cobra.Command{
	Use: "build",
	Short: "go build like command",
	DisableFlagParsing: true,
	Run: func(cmd *cobra.Command, args []string {
		err := runBuild(args)
		if err != nil {
			log.Fatal(err)
		}
	})
}

func init() {
	rootCmd.AddCommand(buildCmd)
}