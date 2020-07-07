local class = require 'pl.class'
local TestClass = require 'test_class'
local TestClassSdmmc = class(TestClass)

function TestClassSdmmc:_init(strTestName, uiTestCase, tLogWriter, strLogLevel)
  self:super(strTestName, uiTestCase, tLogWriter, strLogLevel)

  local P = self.P
  self:__parameter {
    P:P('plugin', 'A pattern for the plugin to use.'):
      required(false),

    P:U32('capacity_kb_min', 'The minimum allowed capacity of the card in kilobytes. A 0 sets no minimum border.'):
      default(0),

    P:U32('capacity_kb_max', 'The maximum allowed capacity of the card in kilobytes. A 0 sets no maximum border.'):
      default(0)
  }
end



function TestClassSdmmc:run()
  local atParameter = self.atParameter
  local tLog = self.tLog

  ----------------------------------------------------------------------
  --
  -- Parse the parameters and collect all options.
  --
  local strPluginPattern = atParameter['plugin']:get()

  local strCapacityKbMin = atParameter["capacity_kb_min"]:get()
  local ulCapacityKbMin = tonumber(strCapacityKbMin)
  if ulCapacityKbMin==nil then
    tLog.error('The parameter "capacity_kb_min" is not a number: %s', strCapacityKbMin)
    error('Parameter error')
  end
  if (ulCapacityKbMin<0) or (ulCapacityKbMin>0xffffffff) then
    tLog.error('The parameter "capacity_kb_min" exceeds an unsigned 32bit value: %d', ulCapacityKbMin)
    error('Parameter error')
  end
  local strCapacityKbMax = atParameter["capacity_kb_max"]:get()
  local ulCapacityKbMax = tonumber(strCapacityKbMax)
  if ulCapacityKbMax==nil then
    tLog.error('The parameter "capacity_kb_max" is not a number: %s', strCapacityKbMax)
    error('Parameter error')
  end
  if (ulCapacityKbMax<0) or (ulCapacityKbMax>0xffffffff) then
    tLog.error('The parameter "capacity_kb_max" exceeds an unsigned 32bit value: %d', ulCapacityKbMax)
    error('Parameter error')
  end
  if ulCapacityKbMin>ulCapacityKbMax then
    tLog.error('The minimum capacity is larger than the maximum: %d > %d', ulCapacityKbMin, ulCapacityKbMax)
    error('Parameter error')
  end

  ----------------------------------------------------------------------
  --
  -- Open the connection to the netX.
  -- (or re-use an existing connection.)
  --
  local tPlugin = tester.getCommonPlugin()
  if tPlugin==nil then
    error("No plug-in selected, nothing to do!")
  end

  local astrBinaryName = {
    [romloader.ROMLOADER_CHIPTYP_NETX4000_RELAXED] = '4000',
    [romloader.ROMLOADER_CHIPTYP_NETX4000_FULL]    = '4000',
    [romloader.ROMLOADER_CHIPTYP_NETX4100_SMALL]   = '4000',
--    [romloader.ROMLOADER_CHIPTYP_NETX500]          = '500',
--    [romloader.ROMLOADER_CHIPTYP_NETX100]          = '500',
--    [romloader.ROMLOADER_CHIPTYP_NETX90_MPW]       = '90_mpw',
--    [romloader.ROMLOADER_CHIPTYP_NETX90]           = '90',
--    [romloader.ROMLOADER_CHIPTYP_NETX56]           = '56',
--    [romloader.ROMLOADER_CHIPTYP_NETX56B]          = '56',
--    [romloader.ROMLOADER_CHIPTYP_NETX50]           = '50',
--    [romloader.ROMLOADER_CHIPTYP_NETX10]           = '10'
  }

  -- Get the binary for the ASIC.
  local tAsicTyp = tPlugin:GetChiptyp()
  local strBinary = astrBinaryName[tAsicTyp]
  if strBinary==nil then
    error('No binary for chip type %s.', tAsicTyp)
  end
  local strNetxBinary = string.format('netx/sdmmc_test_netx%s.bin', strBinary)

  local aParameter = {
    0,
    ulCapacityKbMin,
    ulCapacityKbMax
  }
  local ulResult = tester.mbin_simple_run(nil, tPlugin, strNetxBinary, aParameter)
  if ulResult~=0 then
    error('The test failed with return code:' .. ulResult)
  end

  print("")
  print(" #######  ##    ## ")
  print("##     ## ##   ##  ")
  print("##     ## ##  ##   ")
  print("##     ## #####    ")
  print("##     ## ##  ##   ")
  print("##     ## ##   ##  ")
  print(" #######  ##    ## ")
  print("")
end


return TestClassSdmmc
