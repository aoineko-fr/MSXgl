//-----------------------------------------------------------------------------
// Get the first non empty character (not space or tab)

//=============================================================================
// DEFINES
//=============================================================================

const fs = require('fs')

//-----------------------------------------------------------------------------
// Constants

const version = '1.0.0'

const lineTypeSection = 1
const lineTypeText = 2
const bComment = true
const bVerbose = true
//-----------------------------------------------------------------------------

// Variables
let outFilename = './localization.h'
let structName = 'g_LocData'

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
function log(str) {
  if (bVerbose) console.log(str)
}

//-----------------------------------------------------------------------------
// Get the first non empty character (not space or tab)
function getFirstChar(str) {
  return str.trim()[0] || 0
}

//-----------------------------------------------------------------------------
// Parse a line and return a descriptor according to line type
function getLineType(str) {
  const obj = {}
  const chr = getFirstChar(str)
  if (chr == '[') {
    const reg = /\[[ \t]*(?<section>[a-zA-Z0-9]+)[ \t]*\]/
    const res = str.match(reg)
    if (res == null) {
      console.log(`Error: Invalid section! ${str}`)
      return null
    }
    obj.type = lineTypeSection
    obj.value = res.groups.section.toUpperCase()
  } else if (chr != 0) {
    const reg = /[ \t]*(?<key>[a-zA-Z0-9]+)[ \t]*\=[ \t]*"(?<value>[^"]+)"/
    const res = str.match(reg)
    if (res == null) {
      console.log(`Error: Invalid text! ${str}`)
      return null
    }
    obj.type = lineTypeText
    obj.key = res.groups.key.toUpperCase()
    obj.value = res.groups.value
  } else return null

  return obj
}

//-----------------------------------------------------------------------------
/**
 * @description Parse a localization file
 * @param {string} filename The localization file to parse
 * @param {object} locTable The localization table
 * @returns {void}
 */
function parseLocFile(filename, locTable) {
  log(`Parsing ${filename}...`)

  const fileData = fs.readFileSync(filename, 'utf8')
  const lines = fileData.split('\n')

  let currentLang = null

  lines.forEach((line) => {
    const obj = getLineType(line)

    if (obj?.type === lineTypeSection) currentLang = obj.value

    if (obj?.type === lineTypeText) {
      if (!locTable[currentLang]) locTable[currentLang] = []
      locTable[currentLang][obj.key] = obj.value
    }
  })
}

//-----------------------------------------------------------------------------
// Display program usage and options
function displayHelp() {
  console.log(
    '\nUsage:',
    '\tMSXloc file1.loc [file2.loc [...]] [options]',
    '\nOptions:',
    '\t-h                           Display this help',
    '\t--help',
    `\t-o {filename}                Select the output filename (default is '${outFilename}')`,
    '\t--output {filename}',
    '\t-v                           Activate verbose mode',
    '\t--verbose',
    `\t-s {structname}              Select the structure name (default is '${structName}')`,
    '\t--struct {structname}'
  )
}

//=============================================================================
// Main programm
//=============================================================================

console.log(`MSXloc ${version}`)

//-- Setup command line overwrite parameters
const inputFiles = []
const cmdArgs = process.argv.slice(2)
cmdArgs.forEach((arg, index) => {
  if (/^-/.test(arg)) {
    const lowerArg = arg.toLowerCase()

    if (/^-(-help|h|\?)$/.test(lowerArg)) {
      displayHelp()
      return
    }

    if (/^-(-verbose|v)$/.test(lowerArg)) {
      bVerbose = true
      return
    }

    if (/^-(-output|o)$/.test(lowerArg)) {
      outFilename = cmdArgs[index + 1]
      return
    }

    if (/^-(-struct|s)$/.test(lowerArg)) {
      structName = cmdArgs[index + 1]
      return
    }
  } else {
    inputFiles.push(arg)
  }
})

//-----------------------------------------------------------------------------
// Parse the input file
const locTable = {}
inputFiles.forEach((filename) => parseLocFile(filename, locTable))

// log(locTable);

//-----------------------------------------------------------------------------
// Analyze localisation data
const langIDs = []
const textIDs = []
langKeys = Object.keys(locTable)

langKeys.forEach((lang) => {
  langIDs.push(lang)
  const textKeys = Object.keys(locTable[lang])
  textKeys.forEach((text) => !textIDs.includes(text) && textIDs.push(text))
})

// log(langIDs);
// log(textIDs);

//-----------------------------------------------------------------------------
// Generate header file
let i
let headContent = `// Generated by MSXloc ${version}\n#pragma once\n#include "core.h"\n\n`

headContent += '// Langage definition\n'
for (i = 0; i < langIDs.length; i++)
  headContent += `#define LANG_${langIDs[i]} ${i}\n`
headContent += `#define LANG_MAX ${i}\n\n`

headContent += '// Text ID definition\n'
for (i = 0; i < textIDs.length; i++)
  headContent += `#define TEXT_${textIDs[i]} ${i}\n`
headContent += `#define TEXT_MAX ${i}\n\n`

headContent += '// Localization data table\n'
headContent += `const c8* ${structName}[LANG_MAX][TEXT_MAX] =\n{\n`

let langKeys = Object.keys(locTable)

langKeys.forEach((lang) => {
  headContent += `\t// ${lang}\n\t{\n`
  textIDs.forEach((text) => {
    if (locTable?.[lang]?.[text] == null) {
      headContent += `\t\tNULL,`
      log(`Warning: Text ID '${text}' not defined for langage '${lang}'`)
    } else headContent += `\t\t"${locTable[lang][text]}",`
    if (bComment) headContent += ` // TEXT_${text}`
    headContent += '\n'
  })
})

headContent += `};`

// log(headContent);
fs.writeFileSync(outFilename, headContent)
