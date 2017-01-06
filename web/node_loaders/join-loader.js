/**
 * join all string groups to header output format
 */
const _ = require("lodash");
function group2string(group){
    if(!group.groups.length){
        // no template, single value
        return `const char _${group.name}[] PROGMEM = "${group.content.trim()}";\n`
            + `#define ${group.name} (FPSTR(_${group.name}))`
    }
    // template value, define separate parts
    return group.content.map(
        (val, i) => `const char _${group.name}_${i}[] PROGMEM = "${val.trim()}";\n`
    ).join("")
    + `#define ${group.name}(${group.groups.join(", ")}) \\\n`
    + `\t (String() + ${_.zip(group.content, group.groups).map(([c, g], i) => {
            let result = `(FPSTR(_${group.name}_${i}))`;
            if(g){
                result += ` + (${g})`;
            }
            return result;
        }).join(" + ")})`;
}
module.exports = function(content){
    let groups = JSON.parse(content.slice(content.indexOf("["), -1));
    groups = _.groupBy(groups, group => group.lang);
    groups = _.sortBy(
        Object.keys(groups),
        l => l === "en"
    ).map(l => groups[l])
    .map((g, index) => `
${g[0].lang === "en" ? "#else" : `${index === 0 ? "#if" : "#elif"} defined LANG_${g[0].lang.toUpperCase()}`} 
${g.map(group2string).join("\n")}
    `.trim());
    const file = `
${groups.join("\n")}
#endif
    `.trim();
    return `module.exports = ${JSON.stringify(file)};`;
};