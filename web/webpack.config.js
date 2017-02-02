/**
 * Remove Webpack's Bootstrap JS overhead from output
 */
class RemoveBootstrapPlugin {
    apply(compiler){
        compiler.plugin("emit", (compilation, callback) => {
            const asset = compilation.assets["web.h"];
            if(!asset){
                console.log("asset not found");
                return;
            }
            const source = asset.source();
            asset.source = () => eval(source);
            callback();
        });
    }
}

module.exports = {
    entry: "./web.html",
    bail: true,
    target: "node",
    output: {
        path: __dirname,
        filename: "web.h"
    },
    plugins: [new RemoveBootstrapPlugin()],
    module: {
        loaders: [
            {test: /\.html$/, loader: "join!template!translate!split!html"}
        ]
    },
    htmlLoader: {
        ignoreCustomComments: [
            // do not remove comments tagging sections
            /\/?HTTP_[A-Z_]+/
        ],
        collapseBooleanAttributes: true,
        collapseWhitespace: true,
        collapseInlineTagWhitespace: true,
        minifyCSS: true,
        minifyJs: true,
        quoteCharacter: "\"",
        removeComments: true
    }
};