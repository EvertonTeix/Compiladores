import qualified Data.Text as T
import qualified Data.Text.IO as TIO
import System.IO (IOMode(WriteMode), withFile, hPutStrLn)
import Control.Monad (forM_, unless)
import Data.Time.Clock (getCurrentTime, diffUTCTime, NominalDiffTime)
import System.Mem (performGC)
import Data.List (minimumBy)
import Data.Ord (comparing)
import Text.Printf (printf)

data Cidade = Cidade
  { x :: Int
  , y :: Int
  } deriving (Show)

-- Função principal
main :: IO ()
main = do
    -- Medir o tempo de início
    startTime <- getCurrentTime
    
    -- Medir a memória usada no início
    performGC  -- Força a coleta de lixo
    startMemory <- getMemoryUsage

    cidades <- lerCidadesDeArquivo "../C/cidades.txt"

    if null cidades
        then putStrLn "Nenhuma cidade carregada."
        else do
            encontrarMelhorCaminhoVizinhoMaisProximo cidades "viagemHaskell.txt"
            salvarDistancias cidades "distanciasHaskell.txt"
            
            -- Medir o tempo e a memória ao final
            endTime <- getCurrentTime
            performGC  -- Força a coleta de lixo
            endMemory <- getMemoryUsage
            
            -- Calcular tempo e memória usados
            let elapsedTime = diffUTCTime endTime startTime
                usedMemory = endMemory - startMemory

            -- Exibir resultados
            putStrLn $ "Tempo de execucao: " ++ formatTime elapsedTime
            putStrLn $ "Memoria usada: " ++ formatMemory usedMemory

-- Função para ler cidades do arquivo
lerCidadesDeArquivo :: FilePath -> IO [Cidade]
lerCidadesDeArquivo nomeArquivo = do
    conteudo <- TIO.readFile nomeArquivo
    let linhas = T.lines conteudo
    return [parseCidade linha | linha <- linhas]

parseCidade :: T.Text -> Cidade
parseCidade linha =
    let str = T.unpack linha
        coordenadasStr = dropWhile (/= '(') str
        coordenadasStr' = takeWhile (/= ')') (drop 1 coordenadasStr)
        [xStr, yStr] = words (map (\c -> if c == ',' then ' ' else c) coordenadasStr')
    in Cidade (read xStr) (read yStr)

-- Função para encontrar o melhor caminho usando o método do vizinho mais próximo a partir de cada cidade
encontrarMelhorCaminhoVizinhoMaisProximo :: [Cidade] -> FilePath -> IO ()
encontrarMelhorCaminhoVizinhoMaisProximo cidades arquivoSaida = do
    let caminhos = [vizinhoMaisProximo cidades start | start <- [0..(length cidades - 1)]]
        distancias = map (calcularDistanciaTotal cidades) caminhos
        melhorIndice = snd $ minimumBy (comparing fst) (zip distancias [0..])
        melhorCaminho = caminhos !! melhorIndice
        melhorDistancia = distancias !! melhorIndice

    withFile arquivoSaida WriteMode $ \h -> do
        -- Escrevendo todos os caminhos e suas distâncias
        forM_ caminhos $ \caminho -> do
            let distanciaAtual = calcularDistanciaTotal cidades caminho
            hPutStrLn h $ "Caminho: " ++ caminhoToString caminho ++ " | Distancia total: " ++ formatDistance distanciaAtual

        hPutStrLn h "\nMelhor caminho encontrado HASKELL:"
        hPutStrLn h $ "Caminho: " ++ caminhoToString melhorCaminho
        hPutStrLn h $ "Distancia total percorrida: " ++ formatDistance melhorDistancia

caminhoToString :: [Int] -> String
caminhoToString caminho = case caminho of
    [] -> ""
    _ -> unwords [show (x + 1) ++ " -> " | x <- init caminho] ++ show (last caminho + 1)

-- Função para calcular o caminho do vizinho mais próximo a partir de um ponto inicial
vizinhoMaisProximo :: [Cidade] -> Int -> [Int]
vizinhoMaisProximo cidades start = go [start] (remove start [0..(length cidades - 1)])
  where
    go caminho [] = caminho ++ [start]  -- Fecha o ciclo
    go caminho resto = go (caminho ++ [proximo]) (remove proximo resto)
      where
        proximo = minimumBy (comparing (distanciaParaUltimo (cidades !! last caminho))) resto
        distanciaParaUltimo cidadeAtual = calcularDistancia cidadeAtual . (cidades !!)

    remove _ [] = []
    remove x (y:ys) | x == y = ys
                    | otherwise = y : remove x ys

-- Função para calcular a distância total de um caminho
calcularDistanciaTotal :: [Cidade] -> [Int] -> Double
calcularDistanciaTotal cidades caminho =
    let distancias = case caminho of
            [] -> []
            [_] -> []
            _ -> zipWith (\a b -> calcularDistancia (cidades !! a) (cidades !! b)) caminho (rotate caminho)
    in sum distancias

rotate :: [a] -> [a]
rotate [] = []
rotate [x] = [x]
rotate (x:xs) = xs ++ [x]

calcularDistancia :: Cidade -> Cidade -> Double
calcularDistancia (Cidade x1 y1) (Cidade x2 y2) =
    sqrt $ fromIntegral ((x1 - x2) ^ 2 + (y1 - y2) ^ 2)

-- Função para salvar distâncias em um arquivo
salvarDistancias :: [Cidade] -> FilePath -> IO ()
salvarDistancias cidades arquivo = withFile arquivo WriteMode $ \h -> do
    hPutStrLn h "Arquivo de distancias HASKELL"
    forM_ (zip [0..] cidades) $ \(i, cidade) -> do
        hPutStrLn h $ "Distancias da Cidade " ++ show (i + 1) ++ " para todas as outras:"
        forM_ (zip [0..] cidades) $ \(j, outraCidade) -> do
            unless (i == j) $ do
                let distancia = calcularDistancia cidade outraCidade
                hPutStrLn h $ "Distancia de Cidade " ++ show (i + 1) ++ " para Cidade " ++ show (j + 1) ++ ": " ++ formatDistance distancia
        hPutStrLn h ""  -- Adiciona uma linha em branco após cada cidade

-- Função para formatar a distância
formatDistance :: Double -> String
formatDistance d = printf "%.2f km" d

-- Função para formatar o tempo
formatTime :: NominalDiffTime -> String
formatTime t = printf "%.2f segundos" (realToFrac t :: Double)

-- Função para formatar a memória usada
formatMemory :: Double -> String
formatMemory m = printf "%.2f MB" (m / 1024.0 / 1024.0)

-- Função para medir o uso de memória (aproximado)
getMemoryUsage :: IO Double
getMemoryUsage = do
    performGC
    -- Placeholder; Medição exata pode ser complexa
    return 0.0
